#include "LogicThread.h"
#include "GameManager.h"


void LogicThread::sendJob()
{
	//send �۾��� �� ��
}

void LogicThread::init()
{
	//init�� ���� �� �� ����
}

void LogicThread::job()
{
	//���⼭ game logic main loop ����
	GameManager::getInstance()->update();
}
