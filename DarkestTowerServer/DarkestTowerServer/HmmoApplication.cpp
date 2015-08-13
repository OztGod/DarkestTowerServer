#include "HmmoApplication.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "GameManager.h"
#include "LogicThread.h"

HmmoApplication* HmmoApplication::instance = nullptr;

HmmoApplication::HmmoApplication(int threadNum_)
	: threadNum(threadNum_)
{
	ClientSessionManager::getInstance()->setMaxConnection(1000);
	GameManager::getInstance();

	instance = this;
}

HmmoApplication::~HmmoApplication()
{
}

int HmmoApplication::run()
{
	printf("start server...\n");

	while (ClientSessionManager::getInstance()->acceptClientSessions(listen))
	{
		//logic thread 분리하자
		GameManager::getInstance()->update();
		Sleep(50);
	}

	for (auto thread : threads)
	{
		thread->wait();
	}

	printf("end server...\n");

	return 0;
}

bool HmmoApplication::init()
{

	//main thread lock order check init
	skylark::TLS::lockOrderChecker = new skylark::LockOrderChecker(0);

	ioPort = new skylark::CompletionPort(10);
	logicPort = new skylark::CompletionPort(10);
	listen = new skylark::Socket(skylark::ConnectType::TCP);

	if (!listen->completeTo(ioPort))
		return false;

	if (!listen->reuseAddr(true))
		return false;

	if (!listen->bind("10.73.43.238", 41010))
		return false;

	if (!listen->listen())
		return false;

	threads.push_back(new LogicThread(1, logicPort));

	for (int i = 2; i <= threadNum + 1; i++)
	{
		auto thread = new skylark::IOThread(i, ioPort);

		threads.push_back(thread);
	}

	ClientSessionManager::getInstance()->prepareClientSessions(ioPort);

	return true;
}

HmmoApplication * HmmoApplication::getInstance()
{
	return instance;
}
