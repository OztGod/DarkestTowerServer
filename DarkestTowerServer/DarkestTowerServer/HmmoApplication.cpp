#include "HmmoApplication.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "GameManager.h"
#include "LogicThread.h"
#include "DBThread.h"
#include "DBHelper.h"

HmmoApplication* HmmoApplication::instance = nullptr;

HmmoApplication::HmmoApplication(int threadNum_, int dbThreadNum_)
	: threadNum(threadNum_), dbThreadNum(dbThreadNum_)
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

	ioPort = new skylark::CompletionPort(INFINITE);
	logicPort = new skylark::CompletionPort(10);
	dbPort = new skylark::CompletionPort(INFINITE);
	listen = new skylark::Socket(skylark::ConnectType::TCP);

	if (!listen->completeTo(ioPort))
		return false;

	if (!listen->reuseAddr(true))
		return false;

	if (!listen->bind("10.73.44.30", 41010))
		return false;

	if (!listen->listen())
		return false;

	for (int i = 0; i < dbThreadNum; i++)
	{
		auto thread = new DBThread(i, dbPort);

		threads.push_back(thread);
	}

	threads.push_back(new LogicThread(dbThreadNum, logicPort));

	for (int i = dbThreadNum + 1; i < threadNum + dbThreadNum + 1; i++)
	{
		auto thread = new skylark::IOThread(i, ioPort);

		threads.push_back(thread);
	}

	//db init
	DBHelper::initialize(L"Driver={SQL Server};Server=GAMESERVER1;Database=Hmmo;UID=sa;PWD=rpatjqj1!", dbThreadNum);

	ClientSessionManager::getInstance()->prepareClientSessions(ioPort);

	return true;
}

HmmoApplication * HmmoApplication::getInstance()
{
	return instance;
}
