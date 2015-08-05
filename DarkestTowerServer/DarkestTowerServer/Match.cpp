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

	players[0]->matchStart(this);
	players[1]->matchStart(this);
}

void Match::ready(std::shared_ptr<Player>& player)
{
	int t = getPlayerIndex(player);

	isReady[t] = true;

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
				data.hp[i] = heroData[t][i].maxHp;
				data.act[i] = heroData[t][i].maxAct;
				data.skillNum[i] = heroData[t][i].skills.size();

				for (int s = 0; s < heroData[t][i].skills.size(); s++)
				{
					data.skillIdx[i] = heroData[t][i].skills[s].id;
					data.skillLevel[i] = heroData[t][i].skills[s].level;
				}

				data.x[i] = heroData[t][i].pos.x;
				data.y[i] = heroData[t][i].pos.y;
			}

			auto context = new PacketContext<GameData>();
			context->packet = data;
			context->session = players[t]->getSession();

			skylark::postContext(HmmoApplication::getInstance()->getIoPort(), context, 0);
		}
		
		isStart = true;
	}
}

void Match::placeHero(std::shared_ptr<Player>& player, int num, Point * points)
{
	int t = getPlayerIndex(player);

	for (int i = 0; i < num; i++)
	{
		heroData[t][i].pos.x = points[i].x;
		heroData[t][i].pos.y = points[i].y;
	}

	ready(player);
}

void Match::moveHero(std::shared_ptr<Player>& player, int idx, Point pos)
{
	int t = getPlayerIndex(player);

	int swapIdx = -1;

	for (int i = 0; i < 4; i++)
	{
		if (heroData[t][i].hp != 0 && heroData[t][i].pos.x == pos.x && heroData[t][i].pos.y == pos.y)
		{
			swapIdx = i;
		}
	}

	Point prevPos = heroData[t][idx].pos;
	heroData[t][idx].pos = pos;
	heroData[t][idx].act -= 1;

	if (swapIdx != -1)
	{
		//자리 바꾸기는 1 더 깎임
		heroData[t][idx].act -= 1;
		
		heroData[t][swapIdx].pos = prevPos;

		//자리 바뀐 애도 바뀌었다는 패킷 보내주기
		ChangeHeroState state;
		
		state.type = Type::CHANGE_HERO_STATE;
		state.idx = swapIdx;
		state.hp = heroData[t][swapIdx].hp;
		state.act = heroData[t][swapIdx].act;
		state.x = heroData[t][swapIdx].pos.x;
		state.y = heroData[t][swapIdx].pos.y;

		auto context = new PacketContext<ChangeHeroState>();
		context->packet = state;
		context->session = player->getSession();

		skylark::postContext(HmmoApplication::getInstance()->getIoPort(), context, 0);
	}

	ChangeHeroState state;

	state.type = Type::CHANGE_HERO_STATE;
	state.idx = idx;
	state.hp = heroData[t][idx].hp;
	state.act = heroData[t][idx].act;
	state.x = heroData[t][idx].pos.x;
	state.y = heroData[t][idx].pos.y;

	auto context = new PacketContext<ChangeHeroState>();
	context->packet = state;
	context->session = player->getSession();

	skylark::postContext(HmmoApplication::getInstance()->getIoPort(), context, 0);
}

void Match::randomHero(std::shared_ptr<Player>& player)
{
	int t = getPlayerIndex(player);

	for (int i = 0; i < 4; i++)
	{
		//hero 랜덤 생성. 아직 영웅 정보 가져와서 초기화 못했으니 일단은 클래스만 랜덤으로.
		heroData[t][i].type = static_cast<HeroClass>(rand() % 6);
		heroData[t][i].maxHp = 5;
		heroData[t][i].maxAct = 5;
		heroData[t][i].hp = heroData[t][i].maxHp;
		heroData[t][i].act = heroData[t][i].maxAct;
		heroData[t][i].pos.x = 0;
		heroData[t][i].pos.y = 0;
	}
}

void Match::getHeroData(std::shared_ptr<Player>& player, OUT std::vector<Hero>& data)
{
	int t = getPlayerIndex(player);

	for (int i = 0; i < 4; i++)
	{
		data.push_back(heroData[t][i]);
	}
}

int Match::getPlayerIndex(std::shared_ptr<Player>& player)
{
	int t = -1;

	for (int i = 0; i < 2; i++)
	{
		if (players[i] == player)
			t = i;
	}

	return t;
}
