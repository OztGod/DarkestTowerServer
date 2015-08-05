#pragma once
#include "GameManager.h"

class ClientSession;
class Match;

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

	ClientSession* getSession() const { return session; }

	void matchStart(Match* match)
	{
		nowMatch = match;
	}

	Match* getMatch()
	{
		return nowMatch;
	}

private:
	ClientSession* session = nullptr;
	Match* nowMatch = nullptr;
	int pid;
};