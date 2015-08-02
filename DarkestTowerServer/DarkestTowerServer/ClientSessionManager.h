#pragma once
#include "Skylark.h"

class ClientSession;

class ClientSessionManager
{
public:
	static ClientSessionManager* getInstance();

	void prepareClientSessions(skylark::CompletionPort* port);
	bool acceptClientSessions(skylark::Socket* listen);

	void returnClientSession(ClientSession* client);

	//처음 초기화할 때 한 번만 호출할 것.
	//TO DO : 한 번만 호출하게끔 강제하자
	void setMaxConnection(int connection) { maxConnection = connection; }

private:
	static ClientSessionManager* instance;

	typedef std::list<ClientSession*> ClientList;
	ClientList freeSessionList;

	skylark::Lock lock;

	int maxConnection = 1000;
	uint64_t currentIssueCount;
	uint64_t currentReturnCount;

	ClientSessionManager() :currentIssueCount(0), currentReturnCount(0), lock(skylark::Lock::Order::FIRST_CLASS) {}
	~ClientSessionManager();
};