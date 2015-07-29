#pragma once
#include "Skylark.h"

class ClientSession : public skylark::Session
{
public:
	ClientSession(skylark::CompletionPort* port, std::size_t sendBufSize, std::size_t recvBufSize);
	~ClientSession() override;

	bool onAccept() override;

private:
};