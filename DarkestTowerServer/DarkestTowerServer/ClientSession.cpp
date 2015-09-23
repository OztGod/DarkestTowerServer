#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "GameManager.h"
#include "Player.h"
#include "Match.h"
#include "HmmoApplication.h"
#include "LogicContext.h"
#include "DBHelper.h"
#include <locale>
#include <codecvt>

ClientSession::ClientSession(skylark::CompletionPort * port, std::size_t sendBufSize, std::size_t recvBufSize)
	:Session(port, sendBufSize, recvBufSize)
{
}

ClientSession::~ClientSession()
{
}

int ClientSession::select(Header header)
{
	return static_cast<int>(header.type);
}


void ClientSession::initHandler()
{
	registerHandler(Type::LOGIN_REQUEST, &ClientSession::onLoginRequest);
	registerHandler(Type::RANDOM_HERO_REQUEST, &ClientSession::onRandomHeroRequest);
	registerHandler(Type::ALLOC_HERO, &ClientSession::onAllocHero);
	registerHandler(Type::TURN_END, &ClientSession::onTurnEnd);
	registerHandler(Type::MOVE_HERO, &ClientSession::onMoveHero);
	registerHandler(Type::SELECT_HERO, &ClientSession::onSelectHero);
	registerHandler(Type::SKILL_RANGE_REQUEST, &ClientSession::onSkillRangeRequest);
	registerHandler(Type::ACT_HERO, &ClientSession::onActHero);
	registerHandler(Type::REGISTER_ACCOUNT_REQUEST, &ClientSession::onRegisterAccount);
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
	packetHandle(this);

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
	GameManager::getInstance()->isValidAccount(packet.id, packet.idLength,
		packet.password, packet.passwordLength, [session = this](int pid, int win, int lose, int elo, int heroNum)
	{
		if (pid == -1)
		{
			return HmmoApplication::getInstance()->getIoPort()->doLambda([session]()
			{
				LoginResponse response;
				response.type = Type::LOGIN_RESPONSE;
				response.result = LoginResult::FAILED;
				return session->sendPacket(response);
			});
		}

		if (pid != -1)
		{
			session->player = std::make_shared<Player>(pid, session);
			session->player->init(win, lose, elo, heroNum);
		}

		return true;
	});
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

void ClientSession::onRegisterAccount(const RegisterAccountRequest & packet)
{
	HmmoApplication::getInstance()->getDBPort()->doLambda([session = this, packet]()
	{
		std::string idStr(packet.id, packet.id + packet.idLength);
		std::string pwdStr(packet.password, packet.password + packet.passwordLength);

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		std::wstring wid = converter.from_bytes(idStr);
		std::wstring wpassword = converter.from_bytes(pwdStr);

		int res = 0;
		bool fetchSuccess = false;

		{
			DBHelper dbHelper;
			
			dbHelper.bindParamText(wid.c_str());
			dbHelper.bindParamText(wpassword.c_str());

			dbHelper.bindResultColumnInt(&res);

			if (dbHelper.execute(L"{ call dbo.spCreateAccount (? , ?) }"))
			{
				fetchSuccess = dbHelper.fetchRow();
			}
		}

		if (fetchSuccess && res != 0)
		{
			//해당 계정에 대한 hero 정보 초기화
			GameManager::getInstance()->initAccount(res);

			HmmoApplication::getInstance()->getIoPort()->doLambda([session]()
			{
				RegisterAccountResponse response;
				response.type = Type::REGISTER_ACCOUNT_RESPONSE;
				response.isSuccess = 1;
				return session->sendPacket(response);
			});
			return true;
		}

		HmmoApplication::getInstance()->getIoPort()->doLambda([session]()
		{
			RegisterAccountResponse response;
			response.type = Type::REGISTER_ACCOUNT_RESPONSE;
			response.isSuccess = 0;
			return session->sendPacket(response);
		});


		return false;
	});
}

void ClientSession::onRequestMatch(const RequestMatch & packet)
{
	HmmoApplication::getInstance()->getLogicPort()->doLambda([p = player]()
	{
		GameManager::getInstance()->addMatchPendingList(p);
		return true;
	});
}

void ClientSession::onCancelMatch(const CancelMatch & packet)
{
	HmmoApplication::getInstance()->getLogicPort()->doLambda([p = player]()
	{
		GameManager::getInstance()->removeMatchPendingList(p);
		return true;
	});
}
