#pragma once
#include <iostream>

const char RandX = 'x';
class ShareMemory
{
	public:
		ShareMemory(int key);
		ShareMemory(int key, int size);
		ShareMemory(const char* name);
		ShareMemory(const char* name, int size);
		virtual ~ShareMemory();

		void* mapShm();
		int unmapShm();
		int delShm();


	private:
		int getShmID(key_t key, int shmSize, int flag);

	private:
		int m_shmID;
		void* m_shmAddr = NULL;

};


