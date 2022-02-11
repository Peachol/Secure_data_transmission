#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "ShareMemory.h"
using namespace std;

ShareMemory::ShareMemory(int key)
{
	m_shmID = shmget(key, 0, 0);
}

ShareMemory::ShareMemory(int key, int size)
{
	m_shmID = shmget(key, size, IPC_CREAT | 0766);	
}

ShareMemory::ShareMemory(const char* name)
{
	key_t key = ftok(name, RandX);
	m_shmID = shmget(key, 0, 0);
}

ShareMemory::ShareMemory(const char* name, int size)
{
	key_t key = ftok(name, RandX);
	m_shmID = shmget(key, size, IPC_CREAT | 0766);
}

ShareMemory::~ShareMemory()
{
	
}

void *ShareMemory::mapShm()
{
	m_shmAddr = shmat(m_shmID, NULL, 0);
	if(m_shmAddr==(void *)-1)
	{
		return NULL;
	}
	
	return m_shmAddr;
}

int ShareMemory::unmapShm()
{
	int ret = shmdt(m_shmAddr);
	
	return ret;
}

int ShareMemory::delShm()
{
	int ret = shmctl(m_shmID, IPC_RMID, NULL);
	
	return ret;
}




