#pragma once
#include "Codec.h"

class CodecFactory
{
public:
	CodecFactory();
	virtual ~CodecFactory();

	virtual Codec* createCodec();
};

