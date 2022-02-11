#include <string.h>
#include <iostream>
#include "SequenceASN1.h"
#include "RequestCodec.h"
using namespace std;

RequestCodec::RequestCodec() : Codec()
{
}

RequestCodec::RequestCodec(RequestMsg* msg) : Codec()
{
	m_msg.cmdType = msg->cmdType;
	strcpy(m_msg.clientId, msg->clientId);
	strcpy(m_msg.authCode, msg->authCode);
	strcpy(m_msg.serverId, msg->serverId);
	strcpy(m_msg.r1, msg->r1);
}

RequestCodec::~RequestCodec()
{
	cout << "RequestCodec destruct ..." << endl;
}

int RequestCodec::msgEncode(char** outData, int &len)
{
	writeHeadNode(m_msg.cmdType);
	writeNextNode(m_msg.clientId, strlen(m_msg.clientId) + 1);
	writeNextNode(m_msg.authCode, strlen(m_msg.authCode) + 1);
	writeNextNode(m_msg.serverId, strlen(m_msg.serverId) + 1);
	writeNextNode(m_msg.r1, strlen(m_msg.r1) + 1);
	packSequence(outData, len);

	return 0;
}

void* RequestCodec::msgDecode(char * inData, int inLen)
{
	unpackSequence((char*)inData, inLen);
	readHeadNode(m_msg.cmdType);
	readNextNode(m_msg.clientId);
	readNextNode(m_msg.authCode);
	readNextNode(m_msg.serverId);
	readNextNode(m_msg.r1);
	cout << "MsgRequest 解码 ok ..." << endl;

	return &m_msg;
}
