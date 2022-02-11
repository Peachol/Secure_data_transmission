#include "RequestFactory.h"
#include "RequestCodec.h"
#include <iostream>
using namespace std;

RequestFactory::RequestFactory()
{
	m_flag = false;
}

RequestFactory::RequestFactory(RequestMsg* msg)
{
	m_flag = true;
	m_request = msg;
}


RequestFactory::~RequestFactory()
{
}

Codec * RequestFactory::createCodec()
{
#if 0
	Codec* codec = NULL;
	if (m_flag)
	{
		codec = new RequestCodec(m_request);
	}
	else
	{
		codec = new RequestCodec();
	}
	m_ptr.reset(codec);
#endif
	if (m_flag)
	{
		//codec = new RequestCodec(m_request);
		m_ptr = make_shared<RequestCodec>(m_request);
	}
	else
	{
		//codec = new RequestCodec();
		m_ptr = make_shared<RequestCodec>();
	}
	// 从智能指针中取值

	return m_ptr.get();
}
