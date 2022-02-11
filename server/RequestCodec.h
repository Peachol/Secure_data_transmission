#pragma once
#include "Codec.h"
struct RequestMsg
{
	//1 密钥协商  	//2 密钥校验; 	// 3 密钥注销
    int		cmdType;				// 报文类型
	char	clientId[12] = { 0 };	// 客户端编号
	char	authCode[65] = { 0 };	// 认证码
	char	serverId[12] = { 0 };	// 服务器端编号 
	char	r1[64] = { 0 };			// 客户端随机数
};

class RequestCodec : public Codec
{
public:
	enum CmdType{NewOrUpdate=1, Check, Revoke, View};
	RequestCodec();
	RequestCodec(RequestMsg* msg);
	~RequestCodec();

	// 重写父类函数
	int msgEncode(char** outData, int &len);
	void* msgDecode(char *inData, int inLen);

private:
	RequestMsg m_msg;
};

