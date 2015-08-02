#pragma once
#include "Skylark.h"
#include "pcPacket.h"

class ClientSession : public skylark::Session
{
public:
	ClientSession(skylark::CompletionPort* port, std::size_t sendBufSize, std::size_t recvBufSize);
	~ClientSession() override;

	bool onAccept() override;
	bool onDisconnect(int reason) override;

	bool onRead() override;
	
	void onRelease() override;

	void sessionReset();

	void onLoginRequest(const LoginRequest& packet);

private:
	//TODO : static으로 바꾸자
	skylark::PacketHandler<Header, static_cast<int>(Type::TYPE_NUM)> handler;
};