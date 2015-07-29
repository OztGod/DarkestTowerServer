#pragma once
#include "Skylark.h"
#include "pcPacket.h"

class ClientSession : public skylark::Session
{
public:
	ClientSession(skylark::CompletionPort* port, std::size_t sendBufSize, std::size_t recvBufSize);
	~ClientSession() override;

	bool onAccept() override;

	bool onRead() override;

	void onLoginRequest(const LoginRequest& packet);

private:
	skylark::PacketHandler<Header, static_cast<int>(Type::TYPE_NUM)> handler;
};