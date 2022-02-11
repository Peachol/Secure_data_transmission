#ifndef ASN1DER_H
#define ASN1DER_H
#include "BaseASN1.h"
#include <string>

class SequenceASN1  : public BaseASN1
{
public:
    SequenceASN1();

    // 添加头结点
    int writeHeadNode(int iValue);
    int writeHeadNode(char* sValue, int len);
    // 添加后继结点
    int writeNextNode(int iValue);
    int writeNextNode(char* sValue, int len);

    // 读头结点数据
    int readHeadNode(int &iValue);
    int readHeadNode(char* sValue);
    // 读后继结点数据
    int readNextNode(int &iValue);
    int readNextNode(char* sValue);

    // 打包链表
    int packSequence(char** outData, int &outLen);
    // 解包链表
    int unpackSequence(char* inData, int inLen);

    // 释放链表
    void freeSequence(ITCAST_ANYBUF* node = NULL);

private:
    ITCAST_ANYBUF* m_header = NULL;
	ITCAST_ANYBUF* m_next   = NULL;
	ITCAST_ANYBUF* m_temp   = NULL;
};

#endif // ASN1DER_H
