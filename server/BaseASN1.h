#ifndef BASEASN1_H
#define BASEASN1_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ItcastLog.h"

// #define ITCASTDER_NoErr 0
// 自定义基础数据类型
typedef int              ITCAST_INT;
typedef unsigned char    ITCAST_UINT8;
typedef unsigned short   ITCAST_UINT16;
typedef unsigned long    ITCAST_UINT32;
typedef signed long      ITCAST_SINT32;
typedef unsigned char    ITASN1_BOOLEAN;

// 数据节点结构体(类)
typedef struct ITCAST_ANYBUF_
{
    ITCAST_UINT8 	  *pData;
    ITCAST_UINT32     dataLen;

    ITCAST_UINT32     unusedBits;	/* for bit string */
    ITCAST_UINT32     memoryType;
    ITCAST_UINT32     dataType;
    struct ITCAST_ANYBUF_ *next;    /* for sequence and set */
    struct ITCAST_ANYBUF_ *prev;
}ITCAST_ANYBUF;

// 自定义复合数据类型
typedef ITCAST_ANYBUF    ITASN1_INTEGER;
typedef ITCAST_ANYBUF    ITASN1_OCTETSTRING;
typedef ITCAST_ANYBUF    ITASN1_BITSTRING;
typedef ITCAST_ANYBUF    ITASN1_PRINTABLESTRING;
/*begin of bmpstring*/
typedef ITCAST_ANYBUF	 ITASN1_BMPSTRING;
/*end of bmpstring*/
typedef ITCAST_ANYBUF    ITASN1_ENUMERATED;
typedef ITCAST_ANYBUF    ITASN1_IA5STRING;
typedef ITCAST_ANYBUF    ITASN1_SEQUENCE;
typedef ITCAST_ANYBUF    ITASN1_SET;

class BaseASN1
{
public:
    // 标记内存类型
    enum MemoryType{StaticMemory=1, MallocMemory=2};
    // 错误描述
    enum DerErrType{
        NoErr=0,
        MemoryErr=200,
        LengthErr,
        LengthNotEqual,
        DataRangeErr,
        InvalidTag
    };
    // 整形数的每一位代表不同的数据类型0-31, 即: tag的值对应的数据类型
    enum DerTag{
        ITCAST_DER_ID_RESERVED,
        ITCAST_DER_ID_BOOLEAN,
        ITCAST_DER_ID_INTEGER,
        ITCAST_DER_ID_BITSTRING,
        ITCAST_DER_ID_OCTETSTRING,
        ITCAST_DER_ID_NULL,
        ITCAST_DER_ID_OBJECT_IDENTIFIER,
        ITCAST_DER_ID_OBJECT_DESCRIPTOR,
        ITCAST_DER_ID_EXTERNAL,
        ITCAST_DER_ID_REAL,
        ITCAST_DER_ID_ENUMERATED,
        ITCAST_DER_ID_EMBEDDED_PDV,
        ITCAST_DER_ID_STRING_UTF8,
        ITCAST_DER_ID_13,
        ITCAST_DER_ID_14,
        ITCAST_DER_ID_15,
        ITCAST_DER_ID_SEQUENCE,
        ITCAST_DER_ID_SET,
        ITCAST_DER_ID_STRING_NUMERIC,
        ITCAST_DER_ID_STRING_PRINTABLE,
        ITCAST_DER_ID_STRING_T61,
        ITCAST_DER_ID_STRING_VIDEOTEX,
        ITCAST_DER_ID_STRING_IA5,
        ITCAST_DER_ID_TIME_UTC,
        ITCAST_DER_ID_TIME_GENERALIZED,
        ITCAST_DER_ID_STRING_GRAPHIC,
        ITCAST_DER_ID_STRING_ISO646,
        ITCAST_DER_ID_STRING_GENERAL,
        ITCAST_DER_ID_STRING_UNIVERSAL,
        ITCAST_DER_ID_29,
        ITCAST_DER_ID_STRING_BMP
    };

    // 构造函数
    BaseASN1();

    ITCAST_INT DER_ItAsn1_WriteInteger(ITCAST_UINT32 integer, ITASN1_INTEGER **ppDerInteger);

    ITCAST_INT DER_ItAsn1_ReadInteger(ITASN1_INTEGER *pDerInteger, ITCAST_UINT32 *pInteger);

    ITCAST_INT DER_ItAsn1_WriteBitString(ITASN1_BITSTRING *pBitString, ITASN1_BITSTRING **ppDerBitString);

    ITCAST_INT DER_ItAsn1_ReadBitString(ITASN1_BITSTRING *pDerBitString, ITASN1_BITSTRING **ppBitString);

    ITCAST_INT DER_ItAsn1_WritePrintableString(ITASN1_PRINTABLESTRING *pPrintString, ITASN1_PRINTABLESTRING **ppDerPrintString);

    ITCAST_INT DER_ItAsn1_ReadPrintableString(ITASN1_PRINTABLESTRING *pDerPrintString, ITASN1_PRINTABLESTRING **ppPrintString);

    ITCAST_INT DER_ItAsn1_WriteSequence(ITASN1_SEQUENCE *pSequence, ITCAST_ANYBUF **ppDerSequence);

    ITCAST_INT DER_ItAsn1_ReadSequence(ITCAST_ANYBUF *pDerSequence, ITASN1_SEQUENCE **ppSequence);

    ITCAST_INT DER_ItAsn1_WriteNull(ITCAST_ANYBUF ** ppDerNull);

    ITCAST_INT DER_ItAsn1_ReadNull(ITCAST_ANYBUF * ppDerNull, ITCAST_UINT8 * pInt);

    // 释放一个序列(链表), pAnyBuf为链表的头结点
    ITCAST_INT DER_ITCAST_FreeQueue(ITCAST_ANYBUF *pAnyBuf);

    // 创建ITCAST_ANYBUF, 将strOrigin写入创建的ITCAST_ANYBUF内存中, 通过pOriginBuf将内存地址传出
    ITCAST_INT DER_ITCAST_String_To_AnyBuf(ITCAST_ANYBUF **pOriginBuf, unsigned char * strOrigin, int strOriginLen);

    int WriteNullSequence(ITCAST_ANYBUF **pOutData);

    // 同 EncodeChar 函数
    int EncodeUnsignedChar(unsigned char *pData, int dataLen, ITCAST_ANYBUF **outBuf);

    int DecodeUnsignedChar(ITCAST_ANYBUF *inBuf, unsigned char **Data, int *pDataLen);

    // pData编码为ITCAST_ANYBUF(有malloc动作), 将新的ITCAST_ANYBUF节点地址赋值给outBuf
    int EncodeChar(char *pData, int dataLen, ITCAST_ANYBUF **outBuf);

    // 解析节点inBuf中的字符串数据, 通过第二个参数Data指针传出
    int DecodeChar(ITCAST_ANYBUF *inBuf, char **Data, int *pDataLen);

private:
    ITCAST_INT DER_ItAsn1_Low_GetTagInfo(
            ITCAST_UINT8 **ppDerData,
            ITCAST_UINT32 **ppTagValue,
            ITCAST_UINT32 **ppTagSize);
    ITCAST_UINT32 DER_ItAsn1_Low_Count_LengthOfSize(ITCAST_UINT32 iLength);
    ITCAST_INT DER_ItAsn1_GetLengthInfo(
            ITCAST_ANYBUF *pDerData,
            int *pLengthValue,
            int *pLengthSize);
    ITCAST_INT DER_ItAsn1_Low_GetLengthInfo(
            ITCAST_UINT8 **ppDerData,
            ITCAST_UINT32 **ppLengthValue,
            ITCAST_UINT32 **ppLengthSize);
    ITCAST_INT DER_ItAsn1_Low_IntToChar(
            ITCAST_UINT32 integer,
            ITCAST_UINT8 **ppData,
            ITCAST_UINT32 **ppLength);
    ITCAST_INT DER_ItAsn1_Low_CharToInt(
            ITCAST_UINT8 *aData,
            ITCAST_UINT32 lLength,
            ITCAST_UINT32 **ppInteger);
    ITCAST_INT DER_ItAsn1_Low_WriteTagAndLength(
            ITCAST_ANYBUF *pAnyIn,
            ITCAST_UINT8 cTag,
            ITCAST_ANYBUF **ppAnyOut,
            ITCAST_UINT8 **ppUint8Value);
    ITCAST_INT DER_ItAsn1_Low_ReadTagAndLength(
            ITCAST_ANYBUF *pAnyIn,
            ITCAST_UINT8 **ppUint8Data,
            ITCAST_ANYBUF **ppAnyOut,
            ITCAST_UINT8 **ppUint8Value);
    ITCAST_INT DER_ItAsn1_WriteCharString(
            ITCAST_ANYBUF *pCharString,
            ITCAST_ANYBUF **ppDerCharString);
    ITCAST_INT DER_ItAsn1_ReadCharString(
            ITCAST_ANYBUF *pDerCharString,
            ITCAST_ANYBUF **ppCharString);
    ITCAST_INT DER_ItAsn1_WriteBmpString(
            ITASN1_BMPSTRING *pBmpString,
            ITASN1_BMPSTRING **ppDerBmpString);
    ITCAST_INT DER_ItAsn1_ReadBmpString(
            ITASN1_BMPSTRING *pDerBmpString,
            ITASN1_BMPSTRING **ppBmpString);
    void DER_DI_FreeAnybuf(ITCAST_ANYBUF  * pAnyBuf);
    // 给一个ITCAST_ANYBUF类型空节点指针分配存储空间
    int DER_CREATE_LOW_ITCAST_ANYBUF(ITCAST_ANYBUF *&point);
    // 内联函数
    inline void DER_ITCAST_Free(void *memblock)
    {
        if(memblock)
        {
            free(memblock);
            memblock = NULL;
        }
    }
    // 计算数据类型对应的tag
    inline ITCAST_UINT32 DER_ITASN1_LOW_IDENTIFIER(ITCAST_UINT8 &cTag)
    {
        return cTag & ITCAST_DER_SHORT_ID_MASK;
    }
    // 在堆上创建一个指定大小的数组, 使用指针的引用 == 使用指针的指针
    inline int DER_ITASN1_LOW_CREATEUINT8(ITCAST_UINT8* &point, ITCAST_UINT32 size)
    {
        point = (ITCAST_UINT8*)malloc(size);
        if (point==NULL)
        {
            return 7002;
        }
        //memset(point,0,size)
        return 0;
    }
    // 在堆上创建一个 ITCAST_UINT32 大小的内存
    // 原来的宏参数是指针, 所以此次应该使用指针的引用
    inline int DER_ITASN1_LOW_CREATEUINT32(ITCAST_UINT32* &point)
    {
        point = (ITCAST_UINT32*)malloc(sizeof(ITCAST_UINT32));
        if (point==NULL)
            return 7002;
        memset(point, 0, sizeof(ITCAST_UINT32));
        return 0;
    }
    // 通过检测iResult的值, 返回错误类型
    inline DerErrType DER_ITASN1_LOW_CHECKERR(ITCAST_INT iResult, DerErrType iErrNumber)
    {
        if (iResult == iErrNumber)
            return iErrNumber;
        return NoErr;
    }
    // 释放节点内存
    inline void DER_DACERT_LOW_FREE_ANYBUF(ITCAST_ANYBUF *point)
    {
        DER_ITCAST_Free(point->pData);
        DER_ITCAST_Free(point);
    }

private:
    ItcastLog m_log;       // log类

	const ITCAST_UINT8 TRUE = 1;
	const ITCAST_UINT8 FALSE = 0;
	/* 标记当前数据是原始数据还是编码数据 */
	const ITCAST_UINT32 ITCAST_DER_CONSTRUCTED = 0x20;
	const ITCAST_UINT32 ITCAST_DER_PRIMITIVE = 0x00;

	/* The encodings for the universal types */
	const ITCAST_UINT32 ITCAST_DER_UNIVERSAL = 0x00;
	const ITCAST_UINT32 ITCAST_DER_APPLICATION = 0x40;
	const ITCAST_UINT32 ITCAST_DER_CONTEXT_SPECIFIC = 0x80;
	const ITCAST_UINT32 ITCAST_DER_PRIVATE = 0xC0;

	const ITCAST_UINT32 ITCAST_DER_RESERVED = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_RESERVED);
	const ITCAST_UINT32 ITCAST_DER_BOOLEAN = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_BOOLEAN);
	const ITCAST_UINT32 ITCAST_DER_INTEGER = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_INTEGER);
	const ITCAST_UINT32 ITCAST_DER_BITSTRING = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_BITSTRING);
	const ITCAST_UINT32 ITCAST_DER_OCTETSTRING = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_OCTETSTRING);
	const ITCAST_UINT32 ITCAST_DER_NULL = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_NULL);
	const ITCAST_UINT32 ITCAST_DER_OBJECT_IDENTIFIER = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_OBJECT_IDENTIFIER);
	const ITCAST_UINT32 ITCAST_DER_OBJECT_DESCRIPTOR = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_OBJECT_DESCRIPTOR);
	const ITCAST_UINT32 ITCAST_DER_EXTERNAL = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_EXTERNAL);
	const ITCAST_UINT32 ITCAST_DER_REAL = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_REAL);
	const ITCAST_UINT32 ITCAST_DER_ENUMERATED = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_ENUMERATED);
	const ITCAST_UINT32 ITCAST_DER_EMBEDDED_PDV = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_EMBEDDED_PDV);
	const ITCAST_UINT32 ITCAST_DER_STRING_UTF8 = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_STRING_UTF8);
	const ITCAST_UINT32 ITCAST_DER_13 = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_13);
	const ITCAST_UINT32 ITCAST_DER_14 = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_14);
	const ITCAST_UINT32 ITCAST_DER_15 = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_15);
	const ITCAST_UINT32 ITCAST_DER_SEQUENCE = (ITCAST_DER_UNIVERSAL | ITCAST_DER_CONSTRUCTED | ITCAST_DER_ID_SEQUENCE);
	const ITCAST_UINT32 ITCAST_DER_SET = (ITCAST_DER_UNIVERSAL | ITCAST_DER_CONSTRUCTED | ITCAST_DER_ID_SET);
	const ITCAST_UINT32 ITCAST_DER_STRING_NUMERIC = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_STRING_NUMERIC);
	const ITCAST_UINT32 ITCAST_DER_STRING_PRINTABLE = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_STRING_PRINTABLE);
	const ITCAST_UINT32 ITCAST_DER_STRING_T61 = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_STRING_T61);
	const ITCAST_UINT32 ITCAST_DER_STRING_VIDEOTEX = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_STRING_VIDEOTEX);
	const ITCAST_UINT32 ITCAST_DER_STRING_IA5 = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_STRING_IA5);
	const ITCAST_UINT32 ITCAST_DER_TIME_UTC = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_TIME_UTC);
	const ITCAST_UINT32 ITCAST_DER_TIME_GENERALIZED = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_TIME_GENERALIZED);
	const ITCAST_UINT32 ITCAST_DER_STRING_GRAPHIC = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_STRING_GRAPHIC);
	const ITCAST_UINT32 ITCAST_DER_STRING_ISO646 = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_STRING_ISO646);
	const ITCAST_UINT32 ITCAST_DER_STRING_GENERAL = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_STRING_GENERAL);
	const ITCAST_UINT32 ITCAST_DER_STRING_UNIVERSAL = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_STRING_UNIVERSAL);
	const ITCAST_UINT32 ITCAST_DER_29 = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_29);
	const ITCAST_UINT32 ITCAST_DER_STRING_BMP = (ITCAST_DER_UNIVERSAL | ITCAST_DER_PRIMITIVE | ITCAST_DER_ID_STRING_BMP);

	/* Masks to extract information from a tag number */
	const ITCAST_UINT32 ITCAST_DER_CLASS_MASK = 0xC0;
	const ITCAST_UINT32 ITCAST_DER_CONSTRUCTED_MASK = 0x20;
	const ITCAST_UINT32 ITCAST_DER_SHORT_ID_MASK = 0x1F;
	const ITCAST_UINT32 ITCAST_DER_FIRST_NOT_ID_MASK = 0x7F;     //xia
	const ITCAST_UINT32 ITCAST_DER_FIRST_YES_ID_MASK = 0x80;     //xia
	const ITCAST_UINT32 ITCAST_DER_ALL_YES_ID_MASK = 0xFF;     //xia
															   /* The maximum size for the short tag number encoding, and the magic value
															   which indicates that a long encoding of the number is being used */
	const ITCAST_UINT32 ITASN1_MAX_SHORT_BER_ID = 30;
	const ITCAST_UINT32 ITASN1_LONG_BER_ID = 0x1F;

};

#endif // BASEASN1_H
