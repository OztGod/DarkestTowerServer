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
	//TODO : ���߿� DB���� �޾ƿ� ������ ������� ���� ������ �����ְԲ�
	classes.clear();

	for (int i = 0; i < 4; i++)
	{
		classes.push_back(static_cast<HeroClass>(rand() % 6));
	}
}

bool GameManager::isValidAccount(const char * id, int idLength, const char * password, int pwdLength)
{
	//���� ���� üũ - ���߿� DB���� üũ�ϰԲ� ����
	if (strncmp("test", id, idLength) == 0 &&
		strncmp("12345", password, pwdLength) == 0)
	{
		return true;
	}

	return false;
}
