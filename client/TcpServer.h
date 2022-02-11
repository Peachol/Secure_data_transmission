#pragma once
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ItcastLog.h"
#include "TcpSocket.h"
// 超时的时间
// static const int TIMEOUT = 10000;

class TcpServer
{
public:
	TcpServer();
	~TcpServer();

	// 服务器设置监听
	int setListen(unsigned short port);
	// 等待并接受客户端连接请求, 默认连接超时时间为10000s
	TcpSocket* acceptConn(int timeout = 10000);
	void closefd();

private:
	int acceptTimeout(int wait_seconds);

private:
	int m_lfd;	// 用于监听的文件描述符
	struct sockaddr_in m_addrCli;
	ItcastLog m_log;
};

