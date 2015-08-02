#include "Match.h"
#include "pcPacket.h"
#include "Player.h"
#include "ClientSession.h"
#include "LogicContext.h"
#include "HmmoApplication.h"

Match::Match(std::shared_ptr<Player>& player1, std::shared_ptr<Player>& player2)
{
	players[0] = player1;
	players[1] = player2;
}

void Match::ready(std::shared_ptr<Player>& player)
{
	if (players[0] == player)
	{
		isReady[0] = true;
	}
	else if (players[1] == player)
	{
		isReady[1] = true;
	}

	//둘 다 ready(영웅 배치 제출)면 진짜 게임 시작(게임 시작 패킷 전송)
	if (isReady[0] && isReady[1])
	{
		GameData data;
		data.classNum = 4;
		data.type = Type::GAME_DATA;

		for (int t = 0; t < 2; t++)
		{
			data.turn = t;
			
			for (int i = 0; i < 4; i++)
			{
				data.classes[i] = heroData[t][i].type;
				data.x[i] = heroData[t][i].pos.x;
				data.y[i] = heroData[t][i].pos.y;
			}

			auto context = new PacketContext<GameData>();
			context->packet = data;
			context->session = players[t]->getSession();

			skylark::postContext(HmmoApplication::getInstance()->getIoPort(), context, 0);
		}
	}
}

void Match::placeHero(std::shared_ptr<Player>& player, int num, Point * points)
{
	int t = -1;
	for (int i = 0; i < 2; i++)
	{
		if (players[i] == player)
			t = i;
	}

	for (int i = 0; i < num; i++)
	{
		heroData[t][i].pos.x = points[i].x;
		heroData[t][i].pos.y = points[i].y;
	}
}
