#include "SequenceASN1.h"

SequenceASN1::SequenceASN1()
{
}

int SequenceASN1::writeHeadNode(int iValue)
{
	DER_ItAsn1_WriteInteger(iValue, &m_header);
	m_next = m_header;

	return 0;
}

int SequenceASN1::writeHeadNode(char * sValue, int len)
{
	EncodeChar(sValue, len, &m_header);
	m_next = m_header;

	return 0;
}

int SequenceASN1::writeNextNode(int iValue)
{
	DER_ItAsn1_WriteInteger(iValue, &m_next->next);
	m_next = m_next->next;

	return 0;
}

int SequenceASN1::writeNextNode(char * sValue, int len)
{
	EncodeChar(sValue, len, &m_next->next);
	m_next = m_next->next;

	return 0;
}

int SequenceASN1::readHeadNode(int & iValue)
{
	DER_ItAsn1_ReadInteger(m_header, (ITCAST_UINT32 *)&iValue);
	m_next = m_header->next;
	return 0;
}

int SequenceASN1::readHeadNode(char * sValue)
{
	DER_ItAsn1_ReadPrintableString(m_header, &m_temp);
	memcpy(sValue, m_temp->pData, m_temp->dataLen);
	DER_ITCAST_FreeQueue(m_temp);
	m_next = m_header->next;
	return 0;
}

int SequenceASN1::readNextNode(int & iValue)
{
	DER_ItAsn1_ReadInteger(m_next, (ITCAST_UINT32 *)&iValue);
	m_next = m_next->next;
	return 0;
}

int SequenceASN1::readNextNode(char * sValue)
{
	DER_ItAsn1_ReadPrintableString(m_next, &m_temp);
	memcpy(sValue, m_temp->pData, m_temp->dataLen);
	DER_ITCAST_FreeQueue(m_temp);
	m_next = m_next->next;
	return 0;
}

int SequenceASN1::packSequence(char ** outData, int & outLen)
{
	DER_ItAsn1_WriteSequence(m_header, &m_temp);
	//传输参数赋值
	*outData = (char *)m_temp->pData;
	outLen = m_temp->dataLen;

	//释放整个链表
	DER_ITCAST_FreeQueue(m_header);

	return 0;
}

int SequenceASN1::unpackSequence(char * inData, int inLen)
{
	DER_ITCAST_String_To_AnyBuf(&m_temp, (unsigned char *)inData, inLen);
	DER_ItAsn1_ReadSequence(m_temp, &m_header);

	DER_ITCAST_FreeQueue(m_temp);
	return 0;
}

void SequenceASN1::freeSequence(ITCAST_ANYBUF * node)
{

}
