#pragma once
#include "TcpSocket.h"
#include "SecKeyShm.h"
#include "TcpServer.h"
#include "RequestCodec.h"
#include "OCCIOP.h"
#include <map>

class ServerInfo
{
public:
	char			serverID[12];	// 服务器端编号
	char			dbUse[24]; 		// 数据库用户名
	char			dbPasswd[24]; 	// 数据库密码
	char			dbSID[24]; 		// 数据库sid

	unsigned short 	sPort;			// 服务器绑定的端口
	int				maxnode;		// 共享内存最大网点树 客户端默认1个
	int 			shmkey;			// 共享内存keyid 创建共享内存时使用	 
};

class ServerOperation
{
public:
	ServerOperation(ServerInfo *info);
	~ServerOperation();

	// 服务器开始工作
	void startWork();
	// 秘钥协商
	int secKeyAgree(RequestMsg* reqmsg, char** outData, int& outLen);
	// 秘钥校验
	int secKeyCheck();
	// 秘钥注销
	int secKeyRevoke();
	// 秘钥查看
	int secKeyView();

	friend void* wroking(void * arg);
	// 线程回调也可使用静态成员函数
	static void* wrokingHard(void* arg);
	static void catchSignal(int num);

private:
	void getRandString(int len, char* randBuf);

private:
	ServerInfo m_info;
	SecKeyShm* m_shm;
	TcpServer m_server;
	TcpSocket* m_client;
	OCCIOP m_occi;
	std::map<pthread_t, TcpSocket*> m_listSocket;
	static bool m_stop;
};

void* wroking(void * arg);

