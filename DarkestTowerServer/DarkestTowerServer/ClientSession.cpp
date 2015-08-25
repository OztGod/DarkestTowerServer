#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "GameManager.h"
#include "Player.h"
#include "Match.h"
#include "HmmoApplication.h"
#include "LogicContext.h"

ClientSession::ClientSession(skylark::CompletionPort * port, std::size_t sendBufSize, std::size_t recvBufSize)
	:Session(port, sendBufSize, recvBufSize), handler(this)
{
	handler.registerSelector([](Header header)
	{
		return static_cast<int>(header.type);
	});

	handler.registerHandler(Type::LOGIN_REQUEST, &ClientSession::onLoginRequest);
	handler.registerHandler(Type::RANDOM_HERO_REQUEST, &ClientSession::onRandomHeroRequest);
	handler.registerHandler(Type::ALLOC_HERO, &ClientSession::onAllocHero);
	handler.registerHandler(Type::TURN_END, &ClientSession::onTurnEnd);
	handler.registerHandler(Type::MOVE_HERO, &ClientSession::onMoveHero);
	handler.registerHandler(Type::SELECT_HERO, &ClientSession::onSelectHero);
	handler.registerHandler(Type::ACT_HERO, &ClientSession::onActHero);
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
		auto context = new skylark::FunctionContext([p = this->player]()
		{
			GameManager::getInstance()->addMatchPendingList(p);
			return true;
		});

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
	auto context = new skylark::FunctionContext([p = this->player]()
	{
		std::vector<HeroClass> classes;

		GameManager::getInstance()->getRandomHeros(p, classes);

		RandomHeroResponse randomResponse;

		randomResponse.type = Type::RANDOM_HERO_RESPONSE;
		randomResponse.heroClass[0] = classes[0];
		randomResponse.heroClass[1] = classes[1];
		randomResponse.heroClass[2] = classes[2];
		randomResponse.heroClass[3] = classes[3];
		
		auto sendContext = new PacketContext<RandomHeroResponse>();

		sendContext->packet = randomResponse;
		sendContext->session = p->getSession();

		if (p->getSession()->isConnected())
		{
			HmmoApplication::getInstance()->getIoPort()->take(sendContext, 0);
			return true;
		}

		return false;
	});

	HmmoApplication::getInstance()->getLogicPort()->take(context, 0);
}

void ClientSession::onAllocHero(const AllocHero & packet)
{
	auto context = new skylark::FunctionContext([p = this->player, packet]()
	{
		std::vector<Point> pos;

		for (int i = 0; i < packet.allocNum; i++)
		{
			pos.push_back(Point(packet.x[i], packet.y[i]));
		}

		p->getMatch()->placeHero(p, pos);

		return true;
	});

	HmmoApplication::getInstance()->getLogicPort()->take(context, 0);
}

void ClientSession::onMoveHero(const MoveHero & packet)
{
	auto context = new skylark::FunctionContext([p = this->player, idx = packet.idx, x = packet.x, y = packet.y]()
	{
		p->getMatch()->moveHero(p, idx, Point(x, y));

		return true;
	});

	HmmoApplication::getInstance()->getLogicPort()->take(context, 0);
}

void ClientSession::onTurnEnd(const TurnEnd & packet)
{
	auto context = new skylark::FunctionContext([p = this->player]()
	{
		p->getMatch()->turnChange(p);

		return true;
	});

	HmmoApplication::getInstance()->getLogicPort()->take(context, 0);
}

void ClientSession::onSelectHero(const SelectHero & packet)
{
	auto context = new skylark::FunctionContext([p = this->player, idx = packet.idx]()
	{
		p->getMatch()->selectHero(p, idx);

		return true;
	});

	HmmoApplication::getInstance()->getLogicPort()->take(context, 0);
}

void ClientSession::onSkillRangeRequest(const SkillRangeRequest & packet)
{
	auto context = new skylark::FunctionContext([p = this->player, heroIdx = packet.heroIdx, skillIdx = packet.skillIdx]()
	{
		p->getMatch()->getSkillRange(p, heroIdx, skillIdx);

		return true;
	});

	HmmoApplication::getInstance()->getLogicPort()->take(context, 0);
}

void ClientSession::onActHero(const ActHero & packet)
{
	auto context = new skylark::FunctionContext([p = this->player, heroIdx = packet.heroIdx, skillIdx = packet.skillIdx,
												pos = Point(packet.x,packet.y)]()
	{
		p->getMatch()->actHero(p, heroIdx, skillIdx, pos);

		return true;
	});

	HmmoApplication::getInstance()->getLogicPort()->take(context, 0);
}
