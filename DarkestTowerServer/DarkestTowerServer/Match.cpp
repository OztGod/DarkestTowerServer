#include "Match.h"
#include "pcPacket.h"
#include "Player.h"
#include "ClientSession.h"
#include "LogicContext.h"
#include "HmmoApplication.h"
#include "Skill.h"

Match::Match()
{
	map.reset();
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

				map.set(heroData[t][i]->getPos().x, heroData[t][i]->getPos().y, t, true);
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
		heroData[t][i]->setPos(points[i]);
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
		if (heroData[t][i]->getHp() != 0 && heroData[t][i]->getPos() == pos)
		{
			swapIdx = i;
		}
	}

	int needAct = 1;

	//�ڸ� �ٲٴ� ���̽��� act 1 �� �ʿ�
	if (swapIdx != -1)
		needAct = 2;

	//act �����ϸ� �ź�
	if (!heroData[t][idx]->consumeAct(needAct))
		return;

	Point prevPos = heroData[t][idx]->getPos();

	//pos �ٲٴ� ��ġ�� �̻��ϸ� �ź�
	if (!heroData[t][idx]->setPos(pos))
		return;

	map.move(prevPos, pos, t);

	if (swapIdx != -1)
	{		
		heroData[t][swapIdx]->setPos(prevPos);

		//�ڸ� �ٲ� �ֵ� �ٲ���ٴ� ��Ŷ �����ֱ�
		sendHeroState(t, swapIdx);
	}

	sendHeroState(t, idx);
}

void Match::randomHero(std::shared_ptr<Player>& player)
{
	int t = getPlayerIndex(player);

	//�׽��� or ���� ���� ���¸� ����� ���� �Ұ�
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

void Match::selectHero(std::shared_ptr<Player>& player, int idx)
{
	//�ش� �÷��̾��� idx��° ������ ���� �ش� ��ġ���� ��� ������ ��� ��ų�� �����ش�
	int t = getPlayerIndex(player);
	std::vector<int> validSkills;

	printf("[player %d] : select hero..", t);

	auto heroPos = heroData[t][idx]->getPos();

	for (int i = 0; i < heroData[t][idx]->getSkillNum(); i++)
	{
		auto skill = heroData[t][idx]->getSkill(i);

		if (skill->isValidActPos(heroPos, map, t) && heroData[t][idx]->getSkillCool(i) == 0)
		{
			validSkills.push_back(i);
		}
	}

	ValidSkills packet;

	packet.type = Type::VALID_SKILLS;
	packet.num = validSkills.size();

	for (int i = 0; i < validSkills.size(); i++)
	{
		packet.idx[i] = validSkills[i];
	}

	sendPacket(t, packet);
}

void Match::getSkillRange(std::shared_ptr<Player>& player, int heroIdx, int skillIdx)
{
	int t = getPlayerIndex(player);

	printf("[player %d]: get Skill Range\n", t);

	auto heroPos = heroData[t][heroIdx]->getPos();
	auto skill = heroData[t][heroIdx]->getSkill(skillIdx);

	//skill �� �� �ִ� ��ġ �ƴϸ� ����
	if (!skill->isActEnable(heroPos, map, t))
	{
		return;
	}

	auto range = skill->getRange(heroPos, map, t);
	auto effect = skill->getEffectRange(heroPos);

	SkillRangeResponse packet;

	packet.type = Type::SKILL_RANGE_RESPONSE;
	packet.rangeNum = range.pos.size();
	packet.isMyField = range.isMyField;
	
	for (int i = 0; i < range.pos.size(); i++)
	{
		packet.rangeX[i] = range.pos[i].x;
		packet.rangeY[i] = range.pos[i].y;
	}

	packet.effectNum = effect.size();

	for (int i = 0; i < effect.size(); i++)
	{
		packet.effectX[i] = effect[i].x;
		packet.effectY[i] = effect[i].y;
	}

	sendPacket(t, packet);
}

void Match::turnChange(std::shared_ptr<Player>& player)
{
	int t = getPlayerIndex(player);

	//���� ���� ���߰ų�, �ڱ� �ϵ� �ƴϸ鼭 �� �ٲܷ��� �� ��� ����
	if (!isStart || nowTurn != t)
		return;

	nowTurn = (nowTurn + 1) % playerNum;

	for (int i = 0; i < 4; i++)
	{
		heroData[nowTurn][i]->turnUpdate();
	}

	UpdateTurn packet;

	packet.type = Type::UPDATE_TURN;
	packet.nowTurn = nowTurn;

	broadcastPacket(packet);
}

void Match::actHero(std::shared_ptr<Player>& player, int heroIdx, int skillIdx, Point pos)
{
	int t = getPlayerIndex(player);

	auto skill = heroData[t][heroIdx]->getSkill(skillIdx);
	auto heroPos = heroData[t][heroIdx]->getPos();

	//skill ��뿡 �ʿ��� act ����Ʈ �������� ���� ��� ����
	if (skill->getAct() > heroData[t][heroIdx]->getAct())
		return;

	//skill�� ���� cool�� ��� ����
	if (heroData[t][heroIdx]->getSkillCool(skillIdx) > 0)
		return;

	//skill ��� �Ұ����� ��ġ�� ��� ����
	if (!skill->isActEnable(heroPos, map, t))
		return;

	bool isEffect = false;

	for (int p = 0; p < playerNum; p++)
	{
		for (int idx = 0; idx < 4; idx++)
		{
			if (skill->doSkill(pos, heroData[t][heroIdx].get(), heroData[p][idx].get(), p == t))
			{
				isEffect = true;

				//��ų ȿ�� �޾����Ƿ� ���ŵ� hero ���� �����ش�
				sendHeroState(p, idx);
				break;
			}
		}

		if (isEffect)
			break;
	}

	//skill ȿ�� ������ ���� ĳ���Ͱ� �ϳ��� ���� ��� ����
	if (!isEffect)
		return;

	//skill ȿ�� ���� �ְ� �ִٸ� ��ų ��������Ƿ� ����� �� act ���ҽ�Ű�� ��ų �� ���� �����
	heroData[t][heroIdx]->setSkillCool(skillIdx, skill->getCool());
	heroData[t][heroIdx]->consumeAct(skill->getAct());

	sendHeroState(t, heroIdx);
}

void Match::sendHeroState(int t, int heroIdx)
{
	ChangeHeroState state;

	state.type = Type::CHANGE_HERO_STATE;
	state.idx = heroIdx;
	state.hp = heroData[t][heroIdx]->getHp();
	state.act = heroData[t][heroIdx]->getAct();
	state.x = heroData[t][heroIdx]->getPos().x;
	state.y = heroData[t][heroIdx]->getPos().y;

	sendPacket(t, state);
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
