#include "SequenceASN1.h"
#include <iostream>
#include <string>
using namespace std;

SequenceASN1::SequenceASN1() : BaseASN1()
{
}

int SequenceASN1::writeHeadNode(int iValue)
{
    int ret = DER_ItAsn1_WriteInteger(iValue, &m_header);
	if (ret != 0)
	{
		return -1;
	}
    m_next = m_header;

    return 0;
}

int SequenceASN1::writeHeadNode(char *sValue, int len)
{
    // 字符串转换为 ITCAST_ANYBUF结构体类型
    // 编码
	int ret = EncodeChar(sValue, len, &m_header);
	if (ret != 0)
	{
		return -1;
	}
    m_next = m_header;

    return 0;
}

int SequenceASN1::writeNextNode(int iValue)
{
	int ret = DER_ItAsn1_WriteInteger(iValue, &m_next->next);
	if (ret != 0)
	{
		return -1;
	}
	m_next = m_next->next;

	return 0;
}

int SequenceASN1::writeNextNode(char *sValue, int len)
{
	// 字符串转换为 ITCAST_ANYBUF结构体类型
	// 编码
	int ret = EncodeChar(sValue, len, &m_next->next);
	if (ret != 0)
	{
		return -1;
	}
	m_next = m_next->next;

	return 0;
}

int SequenceASN1::readHeadNode(int &iValue)
{
    ITCAST_UINT32 number = 0;
    // 整形数保存在head节点中, 结构体解码
    int ret = DER_ItAsn1_ReadInteger(m_header, &number);
	if (ret != 0)
	{
		return -1;
	}
    // 指针后移
    m_next = m_header->next;
    iValue = number;

    return 0;
}

int SequenceASN1::readHeadNode(char* sValue)
{
    // 原始字符串保存在head节点中, 结构体解码
    DER_ItAsn1_ReadPrintableString(m_header, &m_temp);
    memcpy(sValue, m_temp->pData, m_temp->dataLen);
	cout << "head node value: " << sValue << endl;
    // 指针后移
    m_next = m_header->next;
	freeSequence(m_temp);

    return 0;
}

int SequenceASN1::readNextNode(int &iValue)
{
    ITCAST_UINT32 number = 0;
    DER_ItAsn1_ReadInteger(m_next, &number);
    m_next = m_next->next;
    iValue = number;

    return 0;
}

int SequenceASN1::readNextNode(char *sValue)
{
    int ret = DER_ItAsn1_ReadPrintableString(m_next, &m_temp);
	if (ret != 0)
	{
		cout << "error, ret = " << ret << endl;
		return -1;
	}
    memcpy(sValue, m_temp->pData, m_temp->dataLen);
    m_next = m_next->next;
	freeSequence(m_temp);

    return 0;
}

int SequenceASN1::packSequence(char** outData, int &outLen)
{
    DER_ItAsn1_WriteSequence(m_header, &m_temp);
    *outData = (char*)m_temp->pData;
    outLen = m_temp->dataLen;

    return 0;
}

int SequenceASN1::unpackSequence(char* inData, int inLen)
{
    // 1. char* -> ITCAST_ANYBUF
    DER_ITCAST_String_To_AnyBuf(&m_temp, (unsigned char*)inData, inLen);
    // 2. m_temp结点数据还原到链表中
    DER_ItAsn1_ReadSequence(m_temp, &m_header);
    freeSequence(m_temp);

    return 0;
}

void SequenceASN1::freeSequence(ITCAST_ANYBUF *node)
{
    if(node == NULL)
    {
        DER_ITCAST_FreeQueue(m_header);
    }
    else
    {
        DER_ITCAST_FreeQueue(node);
    }
}



