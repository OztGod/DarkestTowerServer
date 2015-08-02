#include "ClientSession.h"
#include "ClientSessionManager.h"

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
	printf("connected!\n");
	return true;
}

bool ClientSession::onDisconnect(int reason)
{
	printf("disconnected!\n");
	return true;
}

bool ClientSession::onRead()
{
	handler.packetHandle(this);

	return preRecv();
}

void ClientSession::onRelease()
{
	ClientSessionManager::getInstance()->returnClientSession(this);
}

void ClientSession::sessionReset()
{
	connected = false;
	refCount = 0;
	
	recvBuffer.bufferReset();

	{
		skylark::Guard guard(sendLock, true);
		sendBuffer.bufferReset();
	}

	socket->setLinger(true, 0);
	delete socket;

	socket = new skylark::Socket(skylark::ConnectType::TCP);
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
