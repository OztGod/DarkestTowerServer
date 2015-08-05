#include "Match.h"
#include "pcPacket.h"
#include "Player.h"
#include "ClientSession.h"
#include "LogicContext.h"
#include "HmmoApplication.h"

Match::Match()
{
}

void Match::registerPlayer(std::shared_ptr<Player>& player)
{
	isReady[playerNum] = false;
	players[playerNum] = player;
	players[playerNum]->matchStart(this);
	playerNum++;
}

void Match::ready(std::shared_ptr<Player>& player)
{
	//�̹� �� ������ ���¸� ����
	if (isStart)
		return;

	int t = getPlayerIndex(player);

	isReady[t] = true;

	//�� �� ready(���� ��ġ ����)�� ��¥ ���� ����(���� ���� ��Ŷ ����)
	if (isAllReady())
	{
		GameData data;
		data.classNum = 4;
		data.type = Type::GAME_DATA;

		for (int t = 0; t < playerNum; t++)
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

			broadcastPacket(data);
		}
		
		isStart = true;
	}
}

void Match::placeHero(std::shared_ptr<Player>& player, int num, Point * points)
{
	int t = getPlayerIndex(player);

	//����� ��ġ ������ �� �����ϱ� ������ ����(���� �ڱ� ������ ����)
	if (isStart || isReady[t])
		return;

	for (int i = 0; i < num; i++)
	{
		heroData[t][i].pos.x = points[i].x;
		heroData[t][i].pos.y = points[i].y;
	}

	ready(player);
}

void Match::moveHero(std::shared_ptr<Player>& player, int idx, Point pos)
{
	//�� �������� ���� �̵� ����
	if (!isStart)
		return;

	int t = getPlayerIndex(player);

	//�ڱ� �ϵ� �ƴѵ� �����Ϸ��� �ϸ� �ź�
	if (t != nowTurn)
		return;

	int swapIdx = -1;

	for (int i = 0; i < 4; i++)
	{
		if (heroData[t][i].hp != 0 && heroData[t][i].pos.x == pos.x && heroData[t][i].pos.y == pos.y)
		{
			swapIdx = i;
		}
	}

	int needAct = 1;

	//�ڸ� �ٲٴ� ���̽��� act 1 �� �ʿ�
	if (swapIdx != -1)
		needAct = 2;

	//act �����ϸ� �ź�
	if (heroData[t][idx].act < needAct)
		return;

	Point prevPos = heroData[t][idx].pos;
	heroData[t][idx].pos = pos;
	heroData[t][idx].act -= needAct;

	if (swapIdx != -1)
	{		
		heroData[t][swapIdx].pos = prevPos;

		//�ڸ� �ٲ� �ֵ� �ٲ���ٴ� ��Ŷ �����ֱ�
		ChangeHeroState state;
		
		state.type = Type::CHANGE_HERO_STATE;
		state.idx = swapIdx;
		state.hp = heroData[t][swapIdx].hp;
		state.act = heroData[t][swapIdx].act;
		state.x = heroData[t][swapIdx].pos.x;
		state.y = heroData[t][swapIdx].pos.y;

		sendPacket(t, state);
	}

	ChangeHeroState state;

	state.type = Type::CHANGE_HERO_STATE;
	state.idx = idx;
	state.hp = heroData[t][idx].hp;
	state.act = heroData[t][idx].act;
	state.x = heroData[t][idx].pos.x;
	state.y = heroData[t][idx].pos.y;

	sendPacket(t, state);
}

void Match::randomHero(std::shared_ptr<Player>& player)
{
	int t = getPlayerIndex(player);

	//�׽��� or ���� ���� ���¸� ����� ���� �Ұ�
	if (isStart || isReady[t])
		return;

	for (int i = 0; i < 4; i++)
	{
		//hero ���� ����. ���� ���� ���� �����ͼ� �ʱ�ȭ �������� �ϴ��� Ŭ������ ��������.
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

void Match::turnChange(std::shared_ptr<Player>& player)
{
	int t = getPlayerIndex(player);

	//���� ���� ���߰ų�, �ڱ� �ϵ� �ƴϸ鼭 �� �ٲܷ��� �� ��� ����
	if (!isStart || nowTurn != t)
		return;

	nowTurn = (nowTurn + 1) % playerNum;

	UpdateTurn packet;

	packet.type = Type::UPDATE_TURN;
	packet.nowTurn = nowTurn;

	broadcastPacket(packet);
}

bool Match::isAllReady()
{
	for (int i = 0; i < playerNum; i++)
	{
		if (!isReady[i])
		{
			return false;
		}
	}
	
	return true;
}

int Match::getPlayerIndex(std::shared_ptr<Player>& player)
{
	int t = -1;

	for (int i = 0; i < playerNum; i++)
	{
		if (players[i] == player)
			t = i;
	}

	return t;
}
