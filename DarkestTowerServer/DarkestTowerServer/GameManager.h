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
	void isValidAccount(const char* id, int idLength, const char* password, int pwdLength, std::function<void(int)> complete);

	void addMatchPendingList(std::shared_ptr<Player> player);

	void login(int pid);
	void logout(int pid);
	void update();

	void createMatch();
	void updateMatch();

	void removePlayerMatchMap(std::shared_ptr<Player> player);

private:
	GameManager();
	~GameManager() = default;

	std::list<std::shared_ptr<Player>> matchPendingList;
	std::vector<Match*> matchList;
	std::map<int, Match*> playerMatchMap;

	static GameManager* instance;
};