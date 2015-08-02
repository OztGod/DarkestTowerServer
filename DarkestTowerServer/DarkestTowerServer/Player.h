#pragma once

class ClientSession;

class Player
{
public:
	Player(int pid_, ClientSession* session_) : pid(pid_), session(session_) {}
	~Player() = default;

private:
	ClientSession* session = nullptr;
	int pid;
};