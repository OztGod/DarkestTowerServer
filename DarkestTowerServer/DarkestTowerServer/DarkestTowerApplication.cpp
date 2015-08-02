#include "DarkestTowerApplication.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "GameManager.h"

DarkestTowerApplication::DarkestTowerApplication(int threadNum_)
	: threadNum(threadNum_)
{
	ClientSessionManager::getInstance()->setMaxConnection(1000);
	GameManager::getInstance();
}

DarkestTowerApplication::~DarkestTowerApplication()
{
}

int DarkestTowerApplication::run()
{
	printf("start server...\n");

	while (ClientSessionManager::getInstance()->acceptClientSessions(listen))
	{
		Sleep(100);
	}

	for (auto thread : threads)
	{
		thread->wait();
	}

	printf("end server...\n");

	return 0;
}

bool DarkestTowerApplication::init()
{

	//main thread lock order check init
	skylark::TLS::lockOrderChecker = new skylark::LockOrderChecker(0);

	mainPort = new skylark::CompletionPort(10);
	listen = new skylark::Socket(skylark::ConnectType::TCP);

	if (!listen->completeTo(mainPort))
		return false;

	if (!listen->reuseAddr(true))
		return false;

	if (!listen->bind("127.0.0.1", 41010))
		return false;

	if (!listen->listen())
		return false;

	for (int i = 1; i <= threadNum; i++)
	{
		auto thread = new skylark::IOThread(i, mainPort);

		threads.push_back(thread);
	}

	ClientSessionManager::getInstance()->prepareClientSessions(mainPort);

	return true;
}
