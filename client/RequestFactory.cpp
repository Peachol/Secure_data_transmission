#include "RequestFactory.h"

RequestFactory::RequestFactory()
{
	m_flag = false;
}

RequestFactory::RequestFactory(RequestMsg * msg)
{
	m_request = msg;
	m_flag = true;
}

RequestFactory::~RequestFactory()
{
}

Codec * RequestFactory::createCodec()
{
	if (m_flag == true)
	{
		//用于编码的类对象
		return new RequestCodec(m_request);
	}
	else
	{
		//用于解码的类对象
		return new RequestCodec;
	}
}
