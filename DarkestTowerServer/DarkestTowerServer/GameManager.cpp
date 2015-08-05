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
	//TODO : ���߿� DB���� �޾ƿ� ������ ������� ���� ������ �����ְԲ�
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
	//���� ���� üũ - ���߿� DB���� üũ�ϰԲ� ����
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
	//match ������� player�� 2�� �̻��̸� ��⸦ �ϳ� �����
	//�ϴ� connect ���� �Ǵ��� ����
	while (matchPendingList.size() >= 2)
	{
		auto player = matchPendingList.front();
		matchPendingList.pop_front();
		auto player2 = matchPendingList.front();
		matchPendingList.pop_front();

		//�� �κ� �ߺ� ��ġ ��� ���� �Ǵ��ϰ�
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
