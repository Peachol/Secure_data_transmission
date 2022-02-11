#include "TcpServer.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

TcpServer::TcpServer()
{
}


TcpServer::~TcpServer()
{
}

int TcpServer::setListen(unsigned short port)
{
	int 	ret = 0;
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);


	m_lfd = socket(PF_INET, SOCK_STREAM, 0);
	if (m_lfd < 0)
	{
		ret = errno;
		m_log.Log(__FILE__, __LINE__, ItcastLog::ERROR, ret, "func socket() err");
		return ret;
	}


	int on = 1;
	ret = setsockopt(m_lfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (ret < 0)
	{
		ret = errno;
		m_log.Log(__FILE__, __LINE__, ItcastLog::ERROR, ret, "func setsockopt() err");
		return ret;
	}


	ret = bind(m_lfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if (ret < 0)
	{
		ret = errno;
		m_log.Log(__FILE__, __LINE__, ItcastLog::ERROR, ret, "func bind() err");
		return ret;
	}

	ret = listen(m_lfd, SOMAXCONN);
	if (ret < 0)
	{
		ret = errno;
		m_log.Log(__FILE__, __LINE__, ItcastLog::ERROR, ret, "func listen() err");
		return ret;
	}

	return ret;
}

TcpSocket* TcpServer::acceptConn(int timeout)
{
	int connfd = acceptTimeout(timeout);
	if (connfd < 0)
	{
		if (connfd == -1 && errno == ETIMEDOUT)
		{
			//printf("func accept_timeout() timeout err:%d \n", ret);
			m_log.Log(__FILE__, __LINE__, ItcastLog::ERROR, connfd, "func acceptConn() TimeOutError");
		}
		else
		{
			m_log.Log(__FILE__, __LINE__, ItcastLog::ERROR, connfd, "func acceptConn() OtherError");
		}
		return NULL;
	}

	return new TcpSocket(connfd);
}

void TcpServer::closefd()
{
	close(m_lfd);
}

int TcpServer::acceptTimeout(int wait_seconds)
{
	int ret;
	socklen_t addrlen = sizeof(struct sockaddr_in);

	if (wait_seconds > 0)
	{
		fd_set accept_fdset;
		struct timeval timeout;
		FD_ZERO(&accept_fdset);
		FD_SET(m_lfd, &accept_fdset);
		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = 0;
		do
		{
			ret = select(m_lfd + 1, &accept_fdset, NULL, NULL, &timeout);
		} while (ret < 0 && errno == EINTR);
		if (ret == -1)
			return -1;
		else if (ret == 0)
		{
			errno = ETIMEDOUT;
			return -1;
		}
	}

	//一但检测出 有select事件发生，表示对等方完成了三次握手，客户端有新连接建立
	//此时再调用accept将不会堵塞
	ret = accept(m_lfd, (struct sockaddr*)&m_addrCli, &addrlen); //返回已连接套接字
	if (ret == -1)
	{
		ret = errno;
		m_log.Log(__FILE__, __LINE__, ItcastLog::ERROR, ret, "func accept() err");

		return ret;
	}




	return ret;
}
