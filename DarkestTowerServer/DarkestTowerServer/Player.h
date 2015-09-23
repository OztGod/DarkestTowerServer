#pragma once
#include "GameManager.h"

class ClientSession;
class Match;

enum class PlayerState
{
	NONE,
	LOBBY,
	GAME
};

class Player
{
public:
	Player(int pid_, ClientSession* session_) : pid(pid_), session(session_) 
	{
		GameManager::getInstance()->login(pid);
	}

	~Player()
	{
		GameManager::getInstance()->logout(pid);
	}

	int getId() const { return pid; }

	void init(int win_, int lose_, int elo_, int heroNum_);

	ClientSession* getSession() const { return session; }

	void matchStart(Match* match)
	{
		state = PlayerState::GAME;
		nowMatch = match;
	}

	void enterLobby();

	Match* getMatch()
	{
		return nowMatch;
	}

	void resetMatch()
	{
		nowMatch = nullptr;
	}

	void invalidHeroInfo(int idx)
	{
		heroInfo[idx].isValid = false;
	}

	void matchEnd(bool isWin);

	const HeroInfo& getHeroInfo(int idx) const { return heroInfo[idx]; }

	PlayerState getState() const { return state; }
private:
	ClientSession* session = nullptr;
	Match* nowMatch = nullptr;
	std::vector<HeroInfo> heroInfo;
	PlayerState state = PlayerState::LOBBY;
	int win;
	int lose;
	int heroNum;
	int elo;
	int pid;
};