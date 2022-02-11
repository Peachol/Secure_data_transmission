#include "RespondCodec.h"
#include <iostream>
using namespace std;

RespondCodec::RespondCodec()
{
}

RespondCodec::RespondCodec(RespondMsg * msg)
{
	memcpy(&m_msg, msg, sizeof(RespondMsg));
}

RespondCodec::~RespondCodec()
{
}

int RespondCodec::msgEncode(char ** outData, int & len)
{
	writeHeadNode(m_msg.rv);
	writeNextNode(m_msg.clientId, strlen(m_msg.clientId) + 1);
	writeNextNode(m_msg.serverId, strlen(m_msg.serverId) + 1);
	writeNextNode(m_msg.r2, strlen(m_msg.r2) + 1);
	writeNextNode(m_msg.seckeyid);
	packSequence(outData, len);

	return 0;
}

void * RespondCodec::msgDecode(char * inData, int inLen)
{
	unpackSequence(inData, inLen);
	readHeadNode(m_msg.rv);
	readNextNode(m_msg.clientId);
	readNextNode(m_msg.serverId);
	readNextNode(m_msg.r2);
	readNextNode(m_msg.seckeyid);

	return &m_msg;
}
