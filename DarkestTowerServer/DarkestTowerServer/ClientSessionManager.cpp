#include "ClientSessionManager.h"
#include "ClientSession.h"

ClientSessionManager* ClientSessionManager::instance = nullptr;

ClientSessionManager * ClientSessionManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new ClientSessionManager();
	}
	
	return instance;
}

void ClientSessionManager::prepareClientSessions(skylark::CompletionPort* port)
{
	for (int i = 0; i < maxConnection; ++i)
	{
		ClientSession* client = new ClientSession(port, 2048, 2048);

		freeSessionList.push_back(client);
	}
}

bool ClientSessionManager::acceptClientSessions(skylark::Socket* listen)
{
	skylark::Guard guard(lock, true);

	while (currentIssueCount - currentReturnCount < maxConnection)
	{
		ClientSession* newClient = freeSessionList.back();
		freeSessionList.pop_back();

		++currentIssueCount;

		newClient->addRef();

		skylark::AcceptContext* context = new skylark::AcceptContext(newClient, listen);

		if (false == newClient->accept(listen, context))
			return false;
	}

	return true;
}

void ClientSessionManager::returnClientSession(ClientSession * client)
{
	skylark::Guard guard(lock, true);

	skylark::CRASH_ASSERT(!client->isConnected() && client->getRefCount() == 0);

	client->sessionReset();

	freeSessionList.push_back(client);

	++currentReturnCount;
}

ClientSessionManager::~ClientSessionManager()
{
	for (auto session : freeSessionList)
	{
		delete session;
	}
}
