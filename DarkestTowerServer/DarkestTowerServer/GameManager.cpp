#include "GameManager.h"
#include "Player.h"
#include "ClientSession.h"
#include "LogicContext.h"
#include "HmmoApplication.h"
#include "Match.h"

GameManager* GameManager::instance = nullptr;

GameManager * GameManager::getInstance()
{
	if (instance == nullptr)
		instance = new GameManager();
	
	return instance;
}

void GameManager::getRandomHeros(std::shared_ptr<Player>& player, OUT std::vector<HeroClass>& classes)
{
	//TODO : 나중에 DB에서 받아온 데이터 기반으로 랜덤 데이터 돌려주게끔
	classes.clear();
	std::vector<Hero> heros;

	playerMatchMap[player->getId()]->randomHero(player);
	playerMatchMap[player->getId()]->getHeroData(player, heros);

	for (int i = 0; i < 4; i++)
	{
		classes.push_back(heros[i].type);
	}
}

int GameManager::isValidAccount(const char * id, int idLength, const char * password, int pwdLength)
{
	//계정 정보 체크 - 나중에 DB에서 체크하게끔 변경

	for (int i = 0; i < 5; i++)
	{
		if (strncmp(accounts[i].name.c_str(), id, idLength) == 0 &&
			strncmp(accounts[i].pwd.c_str(), password, pwdLength) == 0 &&
			accounts[i].isConnected == false)
		{
			return i;
		}
	}

	return -1;
}

void GameManager::addMatchPendingList(std::shared_ptr<Player>& player)
{
	matchPendingList.push_back(player);
}

void GameManager::login(int pid)
{
	accounts[pid].isConnected = true;
}

void GameManager::logout(int pid)
{
	accounts[pid].isConnected = false;
}

void GameManager::update()
{
	createMatch();
}

void GameManager::createMatch()
{
	//match 대기중인 player가 2명 이상이면 경기를 하나 만든다
	while (matchPendingList.size() >= 2)
	{
		auto player = matchPendingList.front();
		matchPendingList.pop_front();

		if (!player->getSession()->isConnected() || player->getMatch() != nullptr)
		{
			continue;
		}

		auto player2 = matchPendingList.front();
		matchPendingList.pop_front();

		if (!player2->getSession()->isConnected() || player2->getMatch() != nullptr)
		{
			matchPendingList.push_front(player);
			continue;
		}

		Match* newMatch = new Match();
		newMatch->registerPlayer(player);
		newMatch->registerPlayer(player2);

		matchList.push_back(newMatch);
		playerMatchMap[player->getId()] = newMatch;
		playerMatchMap[player2->getId()] = newMatch;

		MatchStart start;
		start.type = Type::MATCH_START;
		start.turn = 0;

		auto context = new PacketContext<MatchStart>();
		context->packet = start;
		context->session = player->getSession();
		skylark::postContext(HmmoApplication::getInstance()->getIoPort(), context, 0);

		start.turn = 1;

		auto context2 = new PacketContext<MatchStart>();
		context2->packet = start;
		context2->session = player2->getSession();
		skylark::postContext(HmmoApplication::getInstance()->getIoPort(), context2, 0);
	}
}

GameManager::GameManager()
{
	for (int i = 0; i < 5; i++)
	{
		Account account;

		account.name = "test" + std::to_string(i);
		account.pwd = "12345";

		accounts.push_back(account);
	}
}
