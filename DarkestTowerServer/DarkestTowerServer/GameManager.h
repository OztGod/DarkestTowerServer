#pragma once
#include "Skylark.h"
#include "pcPacket.h"

class Player;
class Match;

class GameManager
{
public:
	static GameManager* getInstance();

	void getRandomHeros(OUT std::vector<HeroClass>& classes);
	int isValidAccount(const char* id, int idLength, const char* password, int pwdLength);

	void addMatchPendingList(std::shared_ptr<Player>& player);

	void update();

private:
	GameManager() = default;
	~GameManager() = default;

	std::list<std::shared_ptr<Player>> matchPendingList;
	std::vector<Match*> matchList;
	std::map<int, Match*> playerMatchMap;

	static GameManager* instance;
};