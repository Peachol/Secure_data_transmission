#include "RespondFactory.h"

RespondFactory::RespondFactory()
{
	m_flag = false;
}

RespondFactory::RespondFactory(RespondMsg *msg)
{
	m_flag = true;
	m_respond = msg;
}


RespondFactory::~RespondFactory()
{
}

Codec * RespondFactory::createCodec()
{
	Codec* codec = NULL;
	if (m_flag)
	{
		codec = new RespondCodec(m_respond);
	}
	else
	{
		codec = new RespondCodec();
	}
	m_ptr.reset(codec);
	return codec;
}
