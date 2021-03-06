#pragma once
#include <memory>
#include <vector>
#include "Hero.h"
#ifndef OUT
#define OUT
#endif
#define MAX_NUM 2

class Player;

enum class MatchState
{
	NONE = 0,
	PICK = 1,
	PLACE = 2,
	GAME = 2
};

class Match
{
public:
	Match();
	void registerPlayer(std::shared_ptr<Player> player);
	void ready(std::shared_ptr<Player> player);

	void placeHero(std::shared_ptr<Player> player, std::vector<Point>& points);
	void moveHero(std::shared_ptr<Player> player, int idx, Point pos);

	void getHeroData(std::shared_ptr<Player> player, OUT std::vector<const Hero*>& data);

	void selectHero(std::shared_ptr<Player> player, int idx);
	void getSkillRange(std::shared_ptr<Player> player, int heroIdx, int skillIdx);

	void turnChange(std::shared_ptr<Player> player);
	void actHero(std::shared_ptr<Player> player, int heroIdx, int skillIdx, Point pos);

	bool isEnd();

	void removeEmptyLine();

	void pickHero(std::shared_ptr<Player> player, int pick1, int pick2);

	void begin();
	void end(int winner);

	void surrender(std::shared_ptr<Player> player);

private:
	void resetPlayer();
	void broadcastHeroState(int t, int heroIdx, bool isMove);

	template<typename Packet>
	void sendPacket(int idx, Packet& p)
	{
		printf("send packet type : %d\n", p.type);
		auto context = new PacketContext<Packet>();
		
		context->packet = p;
		context->session = players[idx]->getSession();

		HmmoApplication::getInstance()->getIoPort()->take(context, 0);
	}

	template<typename Packet>
	void broadcastPacket(Packet& p)
	{
		printf("broadcast packet type : %d\n", p.type);
		for (int i = 0; i < playerNum; i++)
		{
			sendPacket(i, p);
		}
	}

	void sendReject(int idx);

	bool isAllReady();

	int getPlayerIndex(std::shared_ptr<Player>& player);
	std::shared_ptr<Player> players[MAX_NUM];
	bool isReady[MAX_NUM];
	int board[MAX_NUM][3][3];
	UHeroVec heroData[MAX_NUM];
	std::vector<int> heroIndex[MAX_NUM];
	MatchState state = MatchState::PICK;
	int playerNum = 0;
	int nowTurn = 0;
	int pick = 0;
};