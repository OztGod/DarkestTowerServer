#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "GameManager.h"
#include "Player.h"

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

	handler.registerHandler<RandomHeroRequest>(static_cast<int>(Type::RANDOM_HERO_REQUEST), [this](const RandomHeroRequest& packet)
	{
		onRandomHeroRequest(packet);
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
	int pid = GameManager::getInstance()
		->isValidAccount(packet.id, packet.idLength,
			packet.password, packet.passwordLength);


	LoginResponse response;
	response.type = Type::LOGIN_RESPONSE;

	if (pid != -1)
	{
		response.result = LoginResult::SUCCESS;
		player = std::make_shared<Player>(pid, this);
	}
	else
	{
		response.result = LoginResult::FAILED;
	}

	sendPacket(response);
}

void ClientSession::onRandomHeroRequest(const RandomHeroRequest & packet)
{
	std::vector<HeroClass> classes;

	GameManager::getInstance()->getRandomHeros(classes);

	RandomHeroResponse randomResponse;

	randomResponse.type = Type::RANDOM_HERO_RESPONSE;
	randomResponse.heroClass[0] = classes[0];
	randomResponse.heroClass[1] = classes[1];
	randomResponse.heroClass[2] = classes[2];
	randomResponse.heroClass[3] = classes[3];

	sendPacket(randomResponse);
}
