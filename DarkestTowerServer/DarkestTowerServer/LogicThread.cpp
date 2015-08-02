#include "LogicThread.h"
#include "GameManager.h"


void LogicThread::sendJob()
{
	//send 작업은 안 함
}

void LogicThread::init()
{
	//init도 딱히 할 거 없다
}

void LogicThread::job()
{
	//여기서 game logic main loop 돌림
	GameManager::getInstance()->update();
}
