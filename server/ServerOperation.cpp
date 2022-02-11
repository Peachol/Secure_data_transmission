#include "ServerOperation.h"
#include <iostream>
#include <pthread.h>
#include <string.h>
#include "RequestFactory.h"
#include "RespondFactory.h"
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <signal.h>
using namespace std;

bool ServerOperation::m_stop = false;	// 静态变量初始化

ServerOperation::ServerOperation(ServerInfo * info)
{
	memcpy(&m_info, info, sizeof(ServerInfo));
	// 创建共享内存对象
	m_shm = new SecKeyShm(info->shmkey, info->maxnode);
	// 连接oracle数据库
	bool bl = m_occi.connectDB("SECMNG", "SECMNG", "192.168.10.145:1521/orcl");
	if (!bl)
	{
		cout << "数据库 Oracle 连接失败..." << endl;
	}
}

ServerOperation::~ServerOperation()
{
	m_shm->delShm();
	delete m_shm;
	// 关闭数据库
	m_occi.closeDB();
}

void ServerOperation::startWork()
{
	// 注册新号捕捉 signal / sigaction
	struct sigaction act;
	act.sa_flags = 0;
	act.sa_handler = catchSignal;
	sigemptyset(&act.sa_mask);
	sigaction(SIGUSR1, &act, NULL);

	// 1. 通过套接字对象设置监听
	m_server.setListen(m_info.sPort);
	// 2. 等待并接受连接请求
	while (1)
	{
		if (m_stop)
		{
			cout << "应用程序即将退出..." << endl;
			break;
		}
		TcpSocket* socket = m_server.acceptConn(3);
		if (socket == NULL)
		{
			cout << "accept 超时或失败" << endl;
			continue;
		}
		cout << "客户端成功连接服务器..." << endl;
		// 创建子线程
		pthread_t pid;
		pthread_create(&pid, NULL, wrokingHard, this);
		// 线程分离, 子线程自己释放pcb
		pthread_detach(pid);
		// key, value插入到map容器
		// https://cplusplus.com
		// http://zh.cppreference.com
		//m_listSocket.insert(pair<pthread_t, TcpSocket*>(pid, socket));
		m_listSocket.insert(make_pair(pid, socket));
	}
}

int ServerOperation::secKeyAgree(RequestMsg * reqMsg, char ** outData, int & outLen)
{
	RespondMsg resMsg;
	// 1. 生成秘钥 = 客户端的随机数+服务器的随机数
	getRandString(sizeof(resMsg.r2), resMsg.r2);
	// 1.1 生成秘钥
	char key[1024] = { 0 };
	unsigned char mdSha[SHA_DIGEST_LENGTH];
	sprintf(key, "%s%s", reqMsg->r1, resMsg.r2);
	SHA1((unsigned char*)key, strlen(key), (unsigned char*)mdSha);
	for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
	{
		sprintf(&key[i * 2], "%02x", mdSha[i]);
	}
	cout << "服务器端生成的秘钥: " << key << endl;

	// 2. 秘钥信息写入数据库
	// 2.1 从数据库中秘钥编号
	resMsg.seckeyid = m_occi.getKeyID();
	NodeSHMInfo shmInfo;
	shmInfo.status = 1;
	shmInfo.seckeyID = resMsg.seckeyid;	// 从数据中读出的
	strcpy(shmInfo.clientID, reqMsg->clientId);
	strcpy(shmInfo.seckey, key);
	strcpy(shmInfo.serverID, m_info.serverID);
	// 2.2 秘钥写入数据库
	bool bl = m_occi.writeSecKey(&shmInfo);
	if (!bl)
	{
		cout << "写秘钥失败..." << endl;
	}
	cout << "写秘钥完成..." << endl;
	m_occi.updataKeyID(resMsg.seckeyid + 1);

	// 3. 秘钥写入共享内存
	m_shm->shmWrite(&shmInfo);

	// 4. 组织给客户端发送的响应数据
	resMsg.rv = 0;	// 0: 成功, -1: 失败
	resMsg.seckeyid = resMsg.seckeyid;
	strcpy(resMsg.clientId, reqMsg->clientId);
	strcpy(resMsg.serverId, m_info.serverID);

	// 5. 数据序列化
	// 6. 传出参数赋值
	CodecFactory* factory = new RespondFactory(&resMsg);
	Codec* codec = factory->createCodec();
	codec->msgEncode(outData, outLen);

	return 0;
}

// 静态函数
void * ServerOperation::wrokingHard(void * arg)
{
	// 1. 不能调用非静态函数或者变量
	// 2. 静态函数不属于对象
	// 3. 要在静态函数中调用一个对象的成功函数或者成员变量
	// 4. 变通方式: 将类对象作为参数传递给静态函数
	// 1. 接收数据
	int recvLen = -1;
	char* recvBuf = NULL;
	ServerOperation *sop = (ServerOperation*)arg;
	pthread_t threadID = pthread_self();
	TcpSocket* socket = sop->m_listSocket[threadID];
	// recvBuf -> 客户端序列化之后的数据
	socket->recvMsg(&recvBuf, recvLen);

	// 2. 序列化之后的数据解码
	CodecFactory* factory = new RequestFactory();
	Codec* codec = factory->createCodec();
	RequestMsg * reqMsg = (RequestMsg*)codec->msgDecode(recvBuf, recvLen);

	// 3. 验证消息认证码 - 原始数据(r1)+秘钥
	char key[1024] = { 0 };
	unsigned int mdLen = -1;
	unsigned char mdHmac[SHA256_DIGEST_LENGTH];
	sprintf(key, "@%s+%s@", sop->m_info.serverID, reqMsg->clientId);
	cout << "原始数据: " << reqMsg->r1 << endl;
	cout << "key: " << key << endl;
	HMAC(EVP_sha256(), key, strlen(key),
		(unsigned char*)reqMsg->r1, strlen(reqMsg->r1), mdHmac, &mdLen);
	for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
	{
		sprintf(&key[i * 2], "%02x", mdHmac[i]);
	}
	cout << "服务器生成的消息认证码: " << key << endl;
	cout << "客户端生成的消息认证码: " << reqMsg->authCode << endl;
#if 1
	// 服务器生成的消息认证码和客户端的进行对比
	if (strcmp(key, reqMsg->authCode) != 0)
	{
		cout << "消息认证码不匹配..." << endl;
		return NULL;
	}
#endif

	// 4. 判断cmdType
	int len = -1;
	char* outData = NULL;
	switch (reqMsg->cmdType)
	{
	case RequestCodec::NewOrUpdate:
		// 秘钥协商函数
		sop->secKeyAgree(reqMsg, &outData, len);
		break;
	case RequestCodec::Check:
		break;
	case RequestCodec::Revoke:
		break;
	default:
		break;
	}
	// 数据发送
	socket->sendMsg(outData, len);

	// 通信完成, 释放内存
	delete factory;
	// 从容器中删除键值对
	//map<pthread_t, TcpSocket*>::iterator it = sop->m_listSocket.find(threadID);
	auto it = sop->m_listSocket.find(threadID);
	sop->m_listSocket.erase(it);
	delete socket;
}

void ServerOperation::catchSignal(int num)
{
	cout << "catch signal num: " << num << endl;
	// 让当前程序终止
	m_stop = true;
}

void ServerOperation::getRandString(int len, char * randBuf)
{
	int flag = -1;
	// 设置随机种子
	srand(time(NULL));
	// 随机字符串: A-Z, a-z, 0-9, 特殊字符(!@#$%^&*()_+=)
	char chars[] = "!@#$%^&*()_+=";
	for (int i = 0; i < len - 1; ++i)
	{
		flag = rand() % 4;
		switch (flag)
		{
		case 0:
			randBuf[i] = 'Z' - rand() % 26;
			break;
		case 1:
			randBuf[i] = 'z' - rand() % 26;
			break;
		case 3:
			randBuf[i] = rand() % 10 + '0';
			break;
		case 2:
			randBuf[i] = chars[rand() % strlen(chars)];
			break;
		default:
			break;
		}
	}
	randBuf[len - 1] = '\0';
}

// 友元函数, 可以在该友元函数中通过对应的类对象调用期私有成员函数或者私有变量
// 子线程 - 进行业务流程处理
void * wroking(void * arg)
{
	// 1. 接收数据
	int recvLen = -1;
	char* recvBuf = NULL;
	ServerOperation *sop = (ServerOperation*)arg;
	pthread_t threadID = pthread_self();
	TcpSocket* socket = sop->m_listSocket[threadID];
	// recvBuf -> 客户端序列化之后的数据
	socket->recvMsg(&recvBuf, recvLen);

	// 2. 序列化之后的数据解码
	CodecFactory* factory = new RequestFactory();
	Codec* codec = factory->createCodec();
	RequestMsg * reqMsg = (RequestMsg*)codec->msgDecode(recvBuf, recvLen);

	// 3. 验证消息认证码 - 原始数据(r1)+秘钥
	char key[1024] = { 0 };
	unsigned int mdLen = -1;
	unsigned char mdHmac[SHA256_DIGEST_LENGTH];
	sprintf(key, "@%s+%s@", sop->m_info.serverID, reqMsg->clientId);
	cout << "原始数据: " << reqMsg->r1 << endl;
	cout << "key: " << key << endl;
	HMAC(EVP_sha256(), key, strlen(key), 
		(unsigned char*)reqMsg->r1, strlen(reqMsg->r1), mdHmac, &mdLen);
	for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
	{
		sprintf(&key[i * 2], "%02x", mdHmac[i]);
	}
	cout << "服务器生成的消息认证码: " << key << endl;
	cout << "客户端生成的消息认证码: " << reqMsg->authCode << endl;
#if 1
	// 服务器生成的消息认证码和客户端的进行对比
	if (strcmp(key, reqMsg->authCode) != 0)
	{
		cout << "消息认证码不匹配..." << endl;
		return NULL;
	}
#endif

	// 4. 判断cmdType
	int len = -1;
	char* outData = NULL;
	switch (reqMsg->cmdType)
	{
	case RequestCodec::NewOrUpdate:
		// 秘钥协商函数
		sop->secKeyAgree(reqMsg, &outData, len);
		break;
	case RequestCodec::Check:
		break;
	case RequestCodec::Revoke:
		break;
	default:
		break;
	}
	// 数据发送
	socket->sendMsg(outData, len);

	// 通信完成, 释放内存
	delete factory;
	// 从容器中删除键值对
	//map<pthread_t, TcpSocket*>::iterator it = sop->m_listSocket.find(threadID);
	auto it = sop->m_listSocket.find(threadID);
	sop->m_listSocket.erase(it);
	delete socket;

	return NULL;
}
