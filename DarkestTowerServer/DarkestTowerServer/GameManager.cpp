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
	if (strncmp("test", id, idLength) == 0 &&
		strncmp("12345", password, pwdLength) == 0)
	{
		return 0;
	}

	if (strncmp("test2", id, idLength) == 0 &&
		strncmp("12345", password, pwdLength) == 0)
	{
		return 1;
	}

	return -1;
}

void GameManager::addMatchPendingList(std::shared_ptr<Player>& player)
{
	matchPendingList.push_back(player);
}

void GameManager::update()
{
	//match 대기중인 player가 2명 이상이면 경기를 하나 만든다
	//일단 connect 여부 판단은 제외
	while (matchPendingList.size() >= 2)
	{
		auto player = matchPendingList.front();
		matchPendingList.pop_front();
		auto player2 = matchPendingList.front();
		matchPendingList.pop_front();

		//이 부분 중복 매치 등등 여부 판단하게
		Match* newMatch = new Match(player, player2);

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

void GameManager::placeHero(std::shared_ptr<Player>& player, int posNum, Point * points)
{
	playerMatchMap[player->getId()]->placeHero(player, posNum, points);
}
