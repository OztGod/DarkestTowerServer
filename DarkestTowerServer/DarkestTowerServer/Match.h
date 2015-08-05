#pragma once
#include <memory>
#include <vector>
#include "Hero.h"
#ifndef OUT
#define OUT
#endif

class Player;

class Match
{
public:
	Match(std::shared_ptr<Player>& player1, std::shared_ptr<Player>& player2);
	void ready(std::shared_ptr<Player>& player);

	void placeHero(std::shared_ptr<Player>& player, int num, Point* points);
	void moveHero(std::shared_ptr<Player>& player, int idx, Point pos);

	void randomHero(std::shared_ptr<Player>& player);
	void getHeroData(std::shared_ptr<Player>& player, OUT std::vector<Hero>& data);

	void turnChange(std::shared_ptr<Player>& player);

private:
	template<typename Packet>
	void sendPacket(int idx, Packet& p)
	{
		auto context = new PacketContext<Packet>();
		
		context->packet = p;
		context->session = players[idx]->getSession();

		skylark::postContext(HmmoApplication::getInstance()->getIoPort(), context, 0);
	}

	template<typename Packet>
	void broadcastPacket(Packet& p)
	{
		for (int i = 0; i < 2; i++)
		{
			sendPacket(i, p);
		}
	}

	int getPlayerIndex(std::shared_ptr<Player>& player);
	std::shared_ptr<Player> players[2];
	bool isReady[2];
	int board[2][3][3];
	Hero heroData[2][4];
	int nowTurn = 0;
	bool isStart = false;
};