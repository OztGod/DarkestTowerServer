#include "GameManager.h"

GameManager* GameManager::instance = nullptr;

GameManager * GameManager::getInstance()
{
	if (instance == nullptr)
		instance = new GameManager();
	
	return instance;
}

void GameManager::getRandomHeros(OUT std::vector<HeroClass>& classes)
{
	//TODO : 나중에 DB에서 받아온 데이터 기반으로 랜덤 데이터 돌려주게끔
	classes.clear();

	for (int i = 0; i < 4; i++)
	{
		classes.push_back(static_cast<HeroClass>(rand() % 6));
	}
}

bool GameManager::isValidAccount(const char * id, int idLength, const char * password, int pwdLength)
{
	//계정 정보 체크 - 나중에 DB에서 체크하게끔 변경
	if (strncmp("test", id, idLength) == 0 &&
		strncmp("12345", password, pwdLength) == 0)
	{
		return true;
	}

	return false;
}
