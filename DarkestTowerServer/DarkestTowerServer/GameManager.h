#pragma once
#include "Skylark.h"
#include "pcPacket.h"
#include "Hero.h"

class Player;
class Match;

class GameManager
{
public:
	static GameManager* getInstance();

	void getRandomHeros(std::shared_ptr<Player>& player, OUT std::vector<HeroClass>& classes);
	int isValidAccount(const char* id, int idLength, const char* password, int pwdLength);

	void addMatchPendingList(std::shared_ptr<Player>& player);

	void update();

	void placeHero(std::shared_ptr<Player>& player, int posNum, Point* points);

private:
	GameManager() = default;
	~GameManager() = default;

	std::list<std::shared_ptr<Player>> matchPendingList;
	std::vector<Match*> matchList;
	std::map<int, Match*> playerMatchMap;

	static GameManager* instance;
};