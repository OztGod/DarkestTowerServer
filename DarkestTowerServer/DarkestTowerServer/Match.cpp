#include "Match.h"
#include "pcPacket.h"
#include "Player.h"
#include "ClientSession.h"
#include "LogicContext.h"
#include "HmmoApplication.h"
#include "Skill.h"

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
	//이미 겜 시작한 상태면 무시
	if (isStart)
		return;

	int t = getPlayerIndex(player);

	isReady[t] = true;

	//둘 다 ready(영웅 배치 제출)면 진짜 게임 시작(게임 시작 패킷 전송)
	if (isAllReady())
	{
		printf("send gameData...\n");
		GameData data;
		data.classNum = 4;
		data.type = Type::GAME_DATA;

		for (int t = 0; t < playerNum; t++)
		{
			data.turn = t;
			
			for (int i = 0; i < 4; i++)
			{
				data.classes[i] = heroData[t][i]->getType();
				data.hp[i] = heroData[t][i]->getMaxHp();
				data.act[i] = heroData[t][i]->getMaxAct();
				data.skillNum[i] = heroData[t][i]->getSkillNum();

				for (int s = 0; s < heroData[t][i]->getSkillNum(); s++)
				{
					data.skillType[i] = heroData[t][i]->getSkill(s)->getType();
					data.skillLevel[i] = heroData[t][i]->getSkill(s)->getLevel();
				}

				data.x[i] = heroData[t][i]->getPos().x;
				data.y[i] = heroData[t][i]->getPos().y;
			}

			broadcastPacket(data);
		}
		
		isStart = true;
	}
}

void Match::placeHero(std::shared_ptr<Player>& player, int num, Point * points)
{
	int t = getPlayerIndex(player);

	//히어로 위치 배정은 겜 시작하기 전에만 가능(레디 박기 전에만 가능)
	if (isStart || isReady[t])
		return;

	for (int i = 0; i < num; i++)
	{
		heroData[t][i]->setPos(points[i]);
	}

	ready(player);
}

void Match::moveHero(std::shared_ptr<Player>& player, int idx, Point pos)
{
	//겜 시작했을 때만 이동 가능
	if (!isStart)
		return;

	int t = getPlayerIndex(player);

	//자기 턴도 아닌데 움직일려고 하면 거부
	if (t != nowTurn)
		return;

	int swapIdx = -1;

	for (int i = 0; i < 4; i++)
	{
		if (heroData[t][i]->getHp() != 0 && heroData[t][i]->getPos() == pos)
		{
			swapIdx = i;
		}
	}

	int needAct = 1;

	//자리 바꾸는 케이스는 act 1 더 필요
	if (swapIdx != -1)
		needAct = 2;

	//act 부족하면 거부
	if (!heroData[t][idx]->consumeAct(needAct))
		return;

	Point prevPos = heroData[t][idx]->getPos();

	//pos 바꾸는 위치가 이상하면 거부
	if (!heroData[t][idx]->setPos(pos))
		return;

	if (swapIdx != -1)
	{		
		heroData[t][swapIdx]->setPos(prevPos);

		//자리 바뀐 애도 바뀌었다는 패킷 보내주기
		ChangeHeroState state;
		
		state.type = Type::CHANGE_HERO_STATE;
		state.idx = swapIdx;
		state.hp = heroData[t][swapIdx]->getHp();
		state.act = heroData[t][swapIdx]->getAct();
		state.x = heroData[t][swapIdx]->getPos().x;
		state.y = heroData[t][swapIdx]->getPos().y;

		sendPacket(t, state);
	}

	ChangeHeroState state;

	state.type = Type::CHANGE_HERO_STATE;
	state.idx = idx;
	state.hp = heroData[t][idx]->getHp();
	state.act = heroData[t][idx]->getAct();
	state.x = heroData[t][idx]->getPos().x;
	state.y = heroData[t][idx]->getPos().y;

	sendPacket(t, state);
}

void Match::randomHero(std::shared_ptr<Player>& player)
{
	int t = getPlayerIndex(player);

	//겜시작 or 레디 박은 상태면 히어로 변경 불가
	if (isStart || isReady[t])
		return;

	for (int i = 0; i < 4; i++)
	{
		heroData[t][i] = getRandomHero();
	}
}

void Match::getHeroData(std::shared_ptr<Player>& player, OUT std::vector<const Hero*>& data)
{
	int t = getPlayerIndex(player);

	for (int i = 0; i < 4; i++)
	{
		data.push_back(heroData[t][i].get());
	}
}

void Match::turnChange(std::shared_ptr<Player>& player)
{
	int t = getPlayerIndex(player);

	//겜이 시작 안했거나, 자기 턴도 아니면서 턴 바꿀려고 한 경우 무시
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
