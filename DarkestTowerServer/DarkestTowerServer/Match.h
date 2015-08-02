#pragma once
#include <memory>
#include <vector>
#include "Hero.h"

class Player;

class Match
{
public:
	Match(std::shared_ptr<Player>& player1, std::shared_ptr<Player>& player2);
	void ready(std::shared_ptr<Player>& player);

	void placeHero(std::shared_ptr<Player>& player, int num, Point* points);

private:
	std::shared_ptr<Player> players[2];
	bool isReady[2];
	int board[2][3][3];
	Hero heroData[2][4];
};