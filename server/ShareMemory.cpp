#include "ShareMemory.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>
using namespace std;

// 通过key打开共享内存
ShareMemory::ShareMemory(int key)
{
	getShmID(key, 0, 0);
}

// 通过传递进来的key创建/打开共享内存
ShareMemory::ShareMemory(int key, int size)
{
	getShmID(key, size, IPC_CREAT | 0664);
}

// 通过路径打开共享内存
ShareMemory::ShareMemory(const char * name)
{
	key_t key = ftok(name, RandX);
	getShmID(key, 0, 0);
}

// 通过路径创建/打开共享内存
ShareMemory::ShareMemory(const char * name, int size)
{
	key_t key = ftok(name, RandX);
	// 创建共享内存
	getShmID(key, size, IPC_CREAT | 0664);
}

ShareMemory::~ShareMemory()
{
}

void * ShareMemory::mapShm()
{
	// 关联当前进程和共享内存
	m_shmAddr = shmat(m_shmID, NULL, 0);
	if (m_shmAddr == (void*)-1)
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

int ShareMemory::getShmID(key_t key, int shmSize, int flag)
{
	cout << "share memory size: " << shmSize << endl;
	m_shmID = shmget(key, shmSize, flag);
	if (m_shmID == -1)
	{
		// 写log日志
	}
	// 共享内存地址初始化
	if (shmSize > 0)
	{
		void* addr = shmat(m_shmID, NULL, 0);
		memset(addr, 0, shmSize);
		shmdt(addr);
	}
	cout << "共享内存创建成功..." << endl;

	return m_shmID;
}
