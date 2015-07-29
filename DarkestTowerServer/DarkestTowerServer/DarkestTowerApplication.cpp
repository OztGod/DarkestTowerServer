#include "DarkestTowerApplication.h"
#include "ClientSession.h"

DarkestTowerApplication::DarkestTowerApplication(int threadNum_)
	: threadNum(threadNum_)
{
}

DarkestTowerApplication::~DarkestTowerApplication()
{
}

int DarkestTowerApplication::run()
{
	//100 clients accept
	for (int i = 0; i < 100; i++)
	{
		ClientSession* session = new ClientSession(mainPort, 4096, 4096);
		skylark::AcceptContext* context = new skylark::AcceptContext(session, listen);

		session->accept(listen, context);
	}

	printf("start server...\n");

	for (auto thread : threads)
	{
		thread->wait();
	}

	printf("end server...\n");

	return 0;
}

bool DarkestTowerApplication::init()
{
	mainPort = new skylark::CompletionPort(10);
	listen = new skylark::Socket(skylark::ConnectType::TCP);

	if (!listen->completeTo(mainPort))
		return false;

	if (!listen->reuseAddr(true))
		return false;

	if (!listen->bind("10.73.43.193", 41010))
		return false;

	if (!listen->listen())
		return false;

	for (int i = 0; i < threadNum; i++)
	{
		auto thread = new skylark::IOThread(i, mainPort);

		threads.push_back(thread);
	}

	return true;
}
