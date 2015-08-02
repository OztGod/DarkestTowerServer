#pragma once
#include "Skylark.h"
#include "pcPacket.h"


class GameManager
{
public:
	static GameManager* getInstance();

	void getRandomHeros(OUT std::vector<HeroClass>& classes);
	bool isValidAccount(const char* id, int idLength, const char* password, int pwdLength);

private:
	GameManager() = default;
	~GameManager() = default;

	static GameManager* instance;
};