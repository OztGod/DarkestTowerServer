#pragma once
#include "Skylark.h"
#include "pcPacket.h"

class Player;

class GameManager
{
public:
	static GameManager* getInstance();

	void getRandomHeros(OUT std::vector<HeroClass>& classes);
	int isValidAccount(const char* id, int idLength, const char* password, int pwdLength);

	void addMatchPendingList(std::shared_ptr<Player>& player);

private:
	GameManager() = default;
	~GameManager() = default;

	std::vector<std::shared_ptr<Player>> matchPendingList;

	static GameManager* instance;
};