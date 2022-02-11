#pragma once
#include "CodecFactory.h"
#include "RespondCodec.h"
#include <memory>

class RespondFactory :
	public CodecFactory
{
public:
	RespondFactory();
	RespondFactory(RespondMsg *msg);
	~RespondFactory();

	Codec* createCodec();

private:
	bool m_flag;
	RespondMsg * m_respond;
	std::shared_ptr<Codec> m_ptr;
};

