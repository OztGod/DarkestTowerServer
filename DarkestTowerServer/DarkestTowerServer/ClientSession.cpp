#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "GameManager.h"
#include "Player.h"
#include "LogicContext.h"
#include "HmmoApplication.h"
#define REGISTER(type, enumValue)\
handler.registerHandler<type>(static_cast<int>(Type::enumValue), [this](const type& packet)\
{\
	on ## type ## (packet); \
});

ClientSession::ClientSession(skylark::CompletionPort * port, std::size_t sendBufSize, std::size_t recvBufSize)
	:Session(port, sendBufSize, recvBufSize)
{
	handler.registerSelector([](Header header)
	{
		return static_cast<int>(header.type);
	});

	REGISTER(LoginRequest, LOGIN_REQUEST);
	REGISTER(RandomHeroRequest, RANDOM_HERO_REQUEST);
	REGISTER(AllocHero, ALLOC_HERO);
	REGISTER(TurnEnd, TURN_END);
	REGISTER(MoveHero, MOVE_HERO);
	REGISTER(SelectHero, SELECT_HERO);
	REGISTER(SkillRangeRequest, SKILL_RANGE_REQUEST);
	REGISTER(ActHero, ACT_HERO);
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

		//일단 로그인 성공하면 무조건 match pending list에 넣는다
		auto context = new AddMatchPendingContext();
		context->player = player;

		HmmoApplication::getInstance()->getLogicPort()->take(context, 0);
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

	HmmoApplication::getInstance()->getLogicPort()->take(context, 0);
}

void ClientSession::onMoveHero(const MoveHero & packet)
{
	auto context = new MoveHeroContext();
	context->player = player;
	context->pos.x = packet.x;
	context->pos.y = packet.y;
	context->idx = packet.idx;

	HmmoApplication::getInstance()->getLogicPort()->take(context, 0);
}

void ClientSession::onTurnEnd(const TurnEnd & packet)
{
	auto context = new TurnEndContext();
	context->player = player;

	HmmoApplication::getInstance()->getLogicPort()->take(context, 0);
}

void ClientSession::onSelectHero(const SelectHero & packet)
{
	auto context = new SelectHeroContext();

	context->player = player;
	context->idx = packet.idx;

	HmmoApplication::getInstance()->getLogicPort()->take(context, 0);
}

void ClientSession::onSkillRangeRequest(const SkillRangeRequest & packet)
{
	auto context = new SkillRangeContext();

	context->player = player;
	context->heroIdx = packet.heroIdx;
	context->skillIdx = packet.skillIdx;

	HmmoApplication::getInstance()->getLogicPort()->take(context, 0);
}

void ClientSession::onActHero(const ActHero & packet)
{
	auto context = new ActHeroContext();

	context->player = player;
	context->heroIdx = packet.heroIdx;
	context->skillIdx = packet.skillIdx;
	context->pos.x = packet.x;
	context->pos.y = packet.y;

	HmmoApplication::getInstance()->getLogicPort()->take(context, 0);
}
