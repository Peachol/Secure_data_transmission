#include "Codec.h"
#include <iostream>
using namespace std;

Codec::Codec() : SequenceASN1()
{
}

Codec::~Codec()
{
	std::cout << "CodeC destruct ..." << std::endl;
}

int Codec::msgEncode(char ** outData, int & len)
{
	return 0;
}

void* Codec::msgDecode(char * inData, int inLen)
{
	return NULL;
}
