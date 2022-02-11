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
		//���ڱ���������
		return new RequestCodec(m_request);
	}
	else
	{
		//���ڽ���������
		return new RequestCodec;
	}
}
