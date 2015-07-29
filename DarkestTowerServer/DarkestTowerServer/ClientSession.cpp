#include "ClientSession.h"

ClientSession::ClientSession(skylark::CompletionPort * port, std::size_t sendBufSize, std::size_t recvBufSize)
	:Session(port, sendBufSize, recvBufSize)
{
}

ClientSession::~ClientSession()
{
}

bool ClientSession::onAccept()
{
	printf("connected!");
	return true;
}
