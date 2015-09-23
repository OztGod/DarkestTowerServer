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

	void getRandomHeros(std::shared_ptr<Player> player, OUT std::vector<HeroClass>& classes);
	using AccountFunc = std::function<void(int, int, int, int, int)>;
	void isValidAccount(const char* id, int idLength, const char* password, int pwdLength, AccountFunc complete);

	void addMatchPendingList(std::shared_ptr<Player> player);
	void removeMatchPendingList(std::shared_ptr<Player> player);

	void login(int pid);
	void logout(int pid);
	void update();

	void createMatch();
	void updateMatch();

	void removePlayerMatchMap(std::shared_ptr<Player> player);

	void initAccount(int pid);

private:
	GameManager();
	~GameManager() = default;

	std::list<std::shared_ptr<Player>> matchPendingList;
	std::vector<Match*> matchList;
	std::map<int, Match*> playerMatchMap;
	int baseHeroNum = 8;

	static GameManager* instance;
};