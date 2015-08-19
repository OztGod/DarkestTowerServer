#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "GameManager.h"
#include "Player.h"
#include "LogicContext.h"
#include "HmmoApplication.h"

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

	handler.registerHandler<AllocHero>(static_cast<int>(Type::ALLOC_HERO), [this](const AllocHero& packet)
	{
		onAllocHero(packet);
	});

	handler.registerHandler<TurnEnd>(static_cast<int>(Type::TURN_END), [this](const TurnEnd& packet)
	{
		onTurnEnd(packet);
	});

	handler.registerHandler<MoveHero>(static_cast<int>(Type::MOVE_HERO), [this](const MoveHero& packet)
	{
		onMoveHero(packet);
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
	player = nullptr;
	
	recvBuffer.bufferReset();

	{
		skylark::Guard guard(sendLock, true);
		sendBuffer.bufferReset();
	}

	socket->setLinger(true, 0);
	delete socket;

	socket = new skylark::Socket(skylark::ConnectType::TCP);
}

void ClientSession::onLoginRequest(const LoginRequest& packet)
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

		printf("login succed! pid = %d\n", pid);

		//일단 로그인 성공하면 무조건 match pending list에 넣는다
		auto context = new AddMatchPendingContext();
		context->player = player;

		skylark::postContext(HmmoApplication::getInstance()->getLogicPort(), context, 0);
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

	GameManager::getInstance()->getRandomHeros(player, classes);

	RandomHeroResponse randomResponse;

	randomResponse.type = Type::RANDOM_HERO_RESPONSE;
	randomResponse.heroClass[0] = classes[0];
	randomResponse.heroClass[1] = classes[1];
	randomResponse.heroClass[2] = classes[2];
	randomResponse.heroClass[3] = classes[3];

	sendPacket(randomResponse);
}

void ClientSession::onAllocHero(const AllocHero & packet)
{
	auto context = new AllocHeroContext();
	context->player = player;
	context->posNum = packet.allocNum;

	printf("allocNum : %d x,y = %d,%d \n", packet.allocNum, packet.x[0], packet.y[0]);
	
	for (int i = 0; i < packet.allocNum; i++)
	{
		context->pos[i].x = packet.x[i];
		context->pos[i].y = packet.y[i];
	}

	skylark::postContext(HmmoApplication::getInstance()->getLogicPort(), context, 0);
}

void ClientSession::onMoveHero(const MoveHero & packet)
{
	auto context = new MoveHeroContext();
	context->player = player;
	context->pos.x = packet.x;
	context->pos.y = packet.y;
	context->idx = packet.idx;

	skylark::postContext(HmmoApplication::getInstance()->getLogicPort(), context, 0);
}

void ClientSession::onTurnEnd(const TurnEnd & packet)
{
	auto context = new TurnEndContext();
	context->player = player;

	skylark::postContext(HmmoApplication::getInstance()->getLogicPort(), context, 0);
}
