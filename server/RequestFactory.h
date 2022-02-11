#pragma once
#include "CodecFactory.h"
#include "RequestCodec.h"
#include <memory>

class RequestFactory :
	public CodecFactory
{
public:
	RequestFactory();
	RequestFactory(RequestMsg* msg);
	~RequestFactory();

	Codec* createCodec();

private:
	bool m_flag = false;
	RequestMsg * m_request;
	std::shared_ptr<RequestCodec> m_ptr;
};

