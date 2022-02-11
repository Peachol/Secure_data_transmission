#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "SecKeyShm.h"
using namespace std;

SecKeyShm::SecKeyShm(int key): ShareMemory(key)
{
	
}

SecKeyShm::SecKeyShm(int key, int maxNode): 
	ShareMemory(key, sizeof(int) + maxNode*sizeof(NodeSHMInfo)), 
	m_maxNode(maxNode)
{
	//���ӹ����ڴ�
	void *p = mapShm();
	
	//��maxNodeд�빲���ڴ��ǰ�ĸ��ֽ�
	memcpy(p, &m_maxNode, sizeof(int));
	
	//��ʼ���ṹ��Ԫ��
	NodeSHMInfo *pNode = (NodeSHMInfo *)((char *)p+sizeof(int));
	memset(pNode, 0x00, sizeof(NodeSHMInfo)*m_maxNode);
	
	//�Ͽ��빲���ڴ�Ĺ���
	unmapShm();
}

SecKeyShm::SecKeyShm(const char* pathName): ShareMemory(pathName)
{
	
}

SecKeyShm::SecKeyShm(const char* pathName, int maxNode):
	ShareMemory(pathName, sizeof(int) + maxNode*sizeof(NodeSHMInfo)), 
	m_maxNode(maxNode)
{
	//���ӹ����ڴ�
	void *p = mapShm();
	
	//��maxNodeд�빲���ڴ��ǰ�ĸ��ֽ�
	memcpy(p, &m_maxNode, sizeof(int));
	
	//��ʼ���ṹ��Ԫ��
	NodeSHMInfo *pNode = (NodeSHMInfo *)((char *)p+sizeof(int));
	memset(pNode, 0x00, sizeof(NodeSHMInfo)*m_maxNode);
	
	//�Ͽ��빲���ڴ�Ĺ���
	unmapShm();
}

SecKeyShm::~SecKeyShm()
{
	
}

int SecKeyShm::shmWrite(NodeSHMInfo* pNodeInfo)
{
	int ret = 0;
	
	//���ӹ����ڴ�
	void * p = mapShm();
	
	//��ù����ڴ��С--ͷ4���ֽڱ�ʾ�����ڴ��С
	memcpy(&m_maxNode, p, sizeof(int));
	//cout << "m_maxNode=="<< m_maxNode << endl;
	
	NodeSHMInfo *pNode = (NodeSHMInfo *)((char *)p + sizeof(int));
	
	//�Ȳ����ͷ��Ѿ����ڹ�
	int i = 0;
	for(i=0; i<m_maxNode; i++)
	{
		if(strcmp(pNode[i].clientID, pNodeInfo->clientID)==0 && 
		   strcmp(pNode[i].serverID, pNodeInfo->serverID)==0)
		{
			//����Կ��Ϣд�뵽���λ��
			memcpy(&pNode[i], pNodeInfo, sizeof(NodeSHMInfo));
			break;
		}
	}
	//cout << "i==" << i << endl;
	
	//��û���ҵ�ԭ����, ����һ������λ��д��
	NodeSHMInfo tmpNode;
	memset(&tmpNode, 0x00, sizeof(tmpNode));
	if(i==m_maxNode)
	{
		for(i=0; i<m_maxNode; i++)
		{
			if(memcmp(&pNode[i], &tmpNode, sizeof(NodeSHMInfo))==0)
			{
				memcpy(&pNode[i], pNodeInfo, sizeof(NodeSHMInfo));
				break;
			}
		}
	}
	//cout << "i==" << i << endl;
	
	
	//û�пռ����	
	if(i==m_maxNode)
	{
		cout << "no space to left" << endl;
		ret = -1;
	}
	//�Ͽ��빲���ڴ�Ĺ���
	unmapShm();
	
	return ret;
}

int SecKeyShm::shmRead(const char* clientID, const char* serverID, NodeSHMInfo* pNodeInfo)
{
	int ret = 0;
	
	//���ӹ����ڴ�
	void * p = mapShm();
	
	//��ù����ڴ��С--ͷ4���ֽڱ�ʾ�����ڴ��С
	memcpy(&m_maxNode, p, sizeof(int));
	cout << "m_maxNode=="<< m_maxNode << endl;
	
	NodeSHMInfo *pNode = (NodeSHMInfo *)((char *)p + sizeof(int));
	
	cout << "clientID:" << clientID << endl;
	cout << "serverID:" << serverID << endl;
	
	int i = 0;
	for(i=0; i<m_maxNode; i++)
	{
		if(strcmp(pNode[i].clientID, clientID)==0 && 
		   strcmp(pNode[i].serverID, serverID)==0)
		{
			//����Կ��Ϣд�뵽���λ��
			memcpy(pNodeInfo, &pNode[i], sizeof(NodeSHMInfo));
			break;
		}
	}
	cout << "i==" << i << endl;
	
	//û�пռ����	
	if(i==m_maxNode)
	{
		cout << "not found data" << endl;
		ret = -1;
	}
	//�Ͽ��빲���ڴ�Ĺ���
	unmapShm();
	
	return ret;
}

void SecKeyShm::printShm()
{
	//���ӹ����ڴ�
	void * p = mapShm();
	
	//��ù����ڴ��С--ͷ4���ֽڱ�ʾ�����ڴ��С
	memcpy(&m_maxNode, p, sizeof(int));
	cout << "m_maxNode=="<< m_maxNode << endl;
	
	NodeSHMInfo *pNode = (NodeSHMInfo *)((char *)p + sizeof(int));
	for(int i=0; i<m_maxNode; i++)
	{
		cout << "--------" << i << "-------" << endl;
		cout << "status:" <<pNode[i].status << endl;
		cout << "seckeyID:" << pNode[i].seckeyID << endl;
		cout << "clientID:" << pNode[i].clientID << endl;
		cout << "serverID:" << pNode[i].serverID << endl;
		cout << "seckey:" << pNode[i].seckey << endl;
		cout << "--------------------" << endl;
	}
	
	//�Ͽ��빲���ڴ�Ĺ���
	unmapShm();
	
	return;
}
