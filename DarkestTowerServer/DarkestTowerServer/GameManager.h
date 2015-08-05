#pragma once
#include "Skylark.h"
#include "pcPacket.h"
#include "Hero.h"

class Player;
class Match;

struct Account
{
	std::string name;
	std::string pwd;
	bool isConnected = false;
};

class GameManager
{
public:
	static GameManager* getInstance();

	void getRandomHeros(std::shared_ptr<Player>& player, OUT std::vector<HeroClass>& classes);
	int isValidAccount(const char* id, int idLength, const char* password, int pwdLength);

	void addMatchPendingList(std::shared_ptr<Player>& player);

	void login(int pid);
	void logout(int pid);
	void update();

	void createMatch();

private:
	GameManager();
	~GameManager() = default;

	std::list<std::shared_ptr<Player>> matchPendingList;
	std::vector<Match*> matchList;
	//юс╫ц
	std::vector<Account> accounts;
	std::map<int, Match*> playerMatchMap;

	static GameManager* instance;
};