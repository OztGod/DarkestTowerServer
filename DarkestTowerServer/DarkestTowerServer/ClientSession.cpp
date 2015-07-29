#include "ClientSession.h"

ClientSession::ClientSession(skylark::CompletionPort * port, std::size_t sendBufSize, std::size_t recvBufSize)
	:Session(port, sendBufSize, recvBufSize)
{
	handler.registerSelector([](Header header)
	{
		return static_cast<int>(header.type);
	});

	handler.registerHandler<LoginRequest>(static_cast<int>(Type::LOGIN_REQUEST), [this](const LoginRequest& packet)
	{
		onLoginRequest(packet);
	});
}

ClientSession::~ClientSession()
{
}

bool ClientSession::onAccept()
{
	printf("connected!");
	return true;
}

bool ClientSession::onRead()
{
	handler.packetHandle(this);

	return preRecv();
}

void ClientSession::onLoginRequest(const LoginRequest & packet)
{
	LoginResponse response;

	response.type = Type::LOGIN_RESPONSE;

	if (strncmp("test", packet.id, packet.idLength) == 0 &&
		strncmp("12345", packet.password, packet.passwordLength) == 0)
	{
		response.result = LoginResult::SUCCESS;
	}
	else
	{
		response.result = LoginResult::FAILED;
	}

	sendPacket(response);
}
