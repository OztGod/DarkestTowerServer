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

void Match::registerPlayer(std::shared_ptr<Player> player)
{
	isReady[playerNum] = false;
	players[playerNum] = player;
	players[playerNum]->matchStart(this);
	playerNum++;
}

void Match::ready(std::shared_ptr<Player> player)
{
	int t = getPlayerIndex(player);

	//ready�� PLACE�ܰ迡���� ����
	if (state != MatchState::PLACE)
	{
		sendReject(t);
		return;
	}

	isReady[t] = true;

	//�� �� ready(���� ��ġ ����)�� ��¥ ���� ����(���� ���� ��Ŷ ����)
	if (isAllReady())
	{
		GameData data;
		data.type = Type::GAME_DATA;

		for (int t = 0; t < playerNum; t++)
		{
			data.turn = t;
			data.classNum = heroData[t].size();
			
			for (int i = 0; i < heroData[t].size(); i++)
			{
				data.classes[i] = heroData[t][i]->getType();
				data.hp[i] = heroData[t][i]->getMaxHp();
				data.act[i] = heroData[t][i]->getMaxAct();
				data.x[i] = heroData[t][i]->getPos().x;
				data.y[i] = heroData[t][i]->getPos().y;
			}

			broadcastPacket(data);

			for (int i = 0; i < heroData[t].size(); i++)
			{
				SkillData skills;

				skills.turn = t;
				skills.type = Type::SKILL_DATA;
				skills.heroIdx = i;
				skills.skillNum = heroData[t][i]->getSkillNum();

				for (int s = 0; s < heroData[t][i]->getSkillNum(); s++)
				{
					skills.skillType[s] = heroData[t][i]->getSkill(s)->getType();
					skills.skillLevel[s] = heroData[t][i]->getSkill(s)->getLevel();
				}

				broadcastPacket(skills);
			}
		}
		
		state = MatchState::GAME;

		UpdateTurn packet;
		
		packet.type = Type::UPDATE_TURN;
		packet.nowTurn = nowTurn;

		broadcastPacket(packet);
	}
}

void Match::placeHero(std::shared_ptr<Player> player, std::vector<Point>& points)
{
	int t = getPlayerIndex(player);

	//����� ��ġ ������ PLACE�϶��� ����
	if (state == MatchState::PLACE || isReady[t])
	{
		sendReject(t);
		return;
	}

	for (int i = 0; i < points.size(); i++)
	{
		heroData[t][i]->setPos(points[i]);
	}

	ready(player);
}

void Match::moveHero(std::shared_ptr<Player> player, int idx, Point pos)
{
	//�� �������� ���� �̵� ����
	int t = getPlayerIndex(player);

	if (state != MatchState::GAME)
	{
		sendReject(t);
		return;
	}

	//�ڱ� �ϵ� �ƴѵ� �����Ϸ��� �ϸ� �ź�
	if (t != nowTurn)
	{
		sendReject(t);
		return;
	}

	int swapIdx = -1;

	for (int i = 0; i < heroData[t].size(); i++)
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
	{
		sendReject(t);
		return;
	}

	Point prevPos = heroData[t][idx]->getPos();

	//pos �ٲٴ� ��ġ�� �̻��ϸ� �ź�
	if (!heroData[t][idx]->move(pos))
	{
		sendReject(t);
		return;
	}

	if (swapIdx != -1)
	{		
		heroData[t][swapIdx]->setPos(prevPos);

		//�ڸ� �ٲ� �ֵ� �ٲ���ٴ� ��Ŷ �����ֱ�
		broadcastHeroState(t, swapIdx, true);
	}

	broadcastHeroState(t, idx, true);

	//�� �ڸ��� �ִ��� Ȯ��
	removeEmptyLine();

	for (int t = 0; t < 2; t++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (!heroData[t][i]->isDead())
				broadcastHeroState(t, i, false);
		}
	}

	selectHero(player, idx);
}

void Match::getHeroData(std::shared_ptr<Player> player, OUT std::vector<const Hero*>& data)
{
	int t = getPlayerIndex(player);

	for (int i = 0; i < heroData[t].size(); i++)
	{
		data.push_back(heroData[t][i].get());
	}
}

void Match::selectHero(std::shared_ptr<Player> player, int idx)
{
	//�ش� �÷��̾��� idx��° ������ ���� �ش� ��ġ���� ��� ������ ��� ��ų�� �����ش�
	int t = getPlayerIndex(player);

	//���� �������� ����
	if (state != MatchState::GAME)
	{
		sendReject(t);
		return;
	}

	std::vector<int> validSkills;

	printf("[player %d] : select hero..\n", t);

	auto heroPos = heroData[t][idx]->getPos();

	printf("valid skills : ");

	for (int i = 0; i < heroData[t][idx]->getSkillNum(); i++)
	{
		auto skill = heroData[t][idx]->getSkill(i);

		if (skill->isValidActPos(heroPos, heroData[t][idx].get(), heroData[t], heroData[(t + 1) % 2]) && heroData[t][idx]->getSkillCool(i) == 0)
		{
			printf("%d ", i);
			validSkills.push_back(i);
		}
	}
	printf("\n");

	ValidSkills packet;

	packet.type = Type::VALID_SKILLS;
	packet.num = validSkills.size();

	for (int i = 0; i < validSkills.size(); i++)
	{
		packet.idx[i] = validSkills[i];
	}

	sendPacket(t, packet);
}

void Match::getSkillRange(std::shared_ptr<Player> player, int heroIdx, int skillIdx)
{
	int t = getPlayerIndex(player);

	if (state != MatchState::GAME)
	{
		sendReject(t);
		return;
	}

	if (heroIdx < 0 || heroIdx >= 4)
	{
		sendReject(t);
		return;
	}

	if (skillIdx < 0 || skillIdx >= heroData[t][heroIdx]->getSkillNum())
	{
		sendReject(t);
		return;
	}

	printf("[player %d]: get Skill Range\n", t);

	auto heroPos = heroData[t][heroIdx]->getPos();
	auto skill = heroData[t][heroIdx]->getSkill(skillIdx);

	//skill �� �� �ִ� ��ġ �ƴϸ� ����
	if (!skill->isActEnable(heroPos, heroData[t], heroData[(t + 1) % 2]))
	{
		sendReject(t);
		return;
	}

	auto range = skill->getRange(heroPos, heroData[t], heroData[(t + 1) % 2]);
	
	SkillRangeResponse packet;

	packet.type = Type::SKILL_RANGE_RESPONSE;
	packet.heroIdx = heroIdx;
	packet.skillIdx = skillIdx;
	packet.rangeNum = range.pos.size();
	packet.isMyField = range.isMyField ? 1 : 0;
	
	for (int i = 0; i < range.pos.size(); i++)
	{
		packet.rangeX[i] = range.pos[i].x;
		packet.rangeY[i] = range.pos[i].y;
	}

	sendPacket(t, packet);

	for (int i = 0; i < range.pos.size(); i++)
	{
		auto effect = skill->getEffectRange(range.pos[i], heroData[t], heroData[(t + 1) % 2]);

		EffectResponse e;
		e.type = Type::EFFECT_RESPONSE;
		e.heroIdx = heroIdx;
		e.skillIdx = skillIdx;
		e.effectNum = effect.size();
		for (int j = 0; j < effect.size(); j++)
		{
			e.effectX[j] = effect[j].x;
			e.effectY[j] = effect[j].y;
		}

		sendPacket(t, e);
	}
}

void Match::turnChange(std::shared_ptr<Player> player)
{
	int t = getPlayerIndex(player);

	//���� ���� ���߰ų�, �ڱ� �ϵ� �ƴϸ鼭 �� �ٲܷ��� �� ��� ����
	if (state != MatchState::GAME || nowTurn != t)
	{
		sendReject(t);
		return;
	}

	nowTurn = (nowTurn + 1) % playerNum;

	for (int i = 0; i < heroData[t].size(); i++)
	{
		//���� �ִ� ����
		if (heroData[nowTurn][i]->isDead())
			continue;

		heroData[nowTurn][i]->turnUpdate();

		//�׾����� �׾��ٴ� ��Ŷ, �ƴϸ� hero state ��Ÿ���� ��Ŷ �߼�
		if (heroData[nowTurn][i]->isDead())
		{
			DeadHero dead;

			dead.type = Type::DEAD_HERO;
			dead.turn = nowTurn;
			dead.heroIdx = i;

			broadcastPacket(dead);
		}
		else
		{
			broadcastHeroState(nowTurn, i, false);
		}
	}

	UpdateTurn packet;

	packet.type = Type::UPDATE_TURN;
	packet.nowTurn = nowTurn;

	broadcastPacket(packet);
}

void Match::actHero(std::shared_ptr<Player> player, int heroIdx, int skillIdx, Point pos)
{
	int t = getPlayerIndex(player);

	if (state != MatchState::GAME)
	{
		sendReject(t);
		return;
	}

	if (heroIdx < 0 || heroIdx >= 4)
	{
		sendReject(t);
		return;
	}

	if (skillIdx < 0 || skillIdx >= heroData[t][heroIdx]->getSkillNum())
	{
		sendReject(t);
		return;
	}

	auto skill = heroData[t][heroIdx]->getSkill(skillIdx);
	auto heroPos = heroData[t][heroIdx]->getPos();

	//skill ��뿡 �ʿ��� act ����Ʈ �������� ���� ��� ����
	if (skill->getAct() > heroData[t][heroIdx]->getAct())
	{
		sendReject(t);
		return;
	}

	//skill�� ���� cool�� ��� ����
	if (heroData[t][heroIdx]->getSkillCool(skillIdx) > 0)
	{
		sendReject(t);
		return;
	}

	//skill ��� �Ұ����� ��ġ�� ��� ����
	if (!skill->isActEnable(heroPos, heroData[t], heroData[(t + 1) % 2]))
	{
		sendReject(t);
		return;
	}

	auto heroList = skill->getHeroInEffect(pos, heroData[t], heroData[(t + 1) % 2]);
			
	//skill ȿ�� ������ ���� ĳ���Ͱ� �ϳ��� ���� ��� ����
	if (heroList.size() == 0)
	{
		sendReject(t);
		return;
	}

	//��ų ��������Ƿ� ����� �� act ���ҽ�Ű�� ��ų �� ���� �����
	heroData[t][heroIdx]->setSkillCool(skillIdx, skill->getCool());
	heroData[t][heroIdx]->consumeAct(skill->getAct());

	SkillShot shot;

	shot.type = Type::SKILL_SHOT;
	shot.turn = t;
	shot.heroIdx = heroIdx;
	shot.skillIdx = skillIdx;
	shot.effectNum = heroList.size();

	for (int i = 0; i < heroList.size(); i++)
	{
		shot.effectTurn[i] = skill->myField() ? t : ((t + 1) % 2);
		shot.effectX[i] = heroData[shot.effectTurn[i]][heroList[i]].get()->getPos().x;
		shot.effectY[i] = heroData[shot.effectTurn[i]][heroList[i]].get()->getPos().y;
	}

	broadcastPacket(shot);

	for (auto& target : heroList)
	{ 
		int turn = skill->myField() ? t : (t + 1) % 2;
		skill->doSkill(pos, heroData[t][heroIdx].get(), heroData[turn][target].get(), heroData[t], heroData[turn]);
		
		if (heroData[turn][target]->isDead())
		{
			DeadHero dead;

			dead.type = Type::DEAD_HERO;
			dead.turn = turn;
			dead.heroIdx = target;

			broadcastPacket(dead);
		}
	}

	//��ų ����� �� ������ ������� Ȯ��.
	removeEmptyLine();

	for (int t = 0; t < 2; t++)
	{
		for (int i = 0; i < 4; i++)
		{
			if(!heroData[t][i]->isDead())
				broadcastHeroState(t, i, false);
		}
	}
}

bool Match::isEnd()
{
	//���� ����� ������ ������ �ڵ����� �������� �ŵд�

	if (!players[0]->getSession()->isConnected())
	{
		end(1);
		return true;
	}

	if (!players[1]->getSession()->isConnected())
	{
		end(0);
		return true;
	}

	for (int t = 0; t < 2; t++)
	{
		bool isEnd = true;

		if (heroData[t].size() == 0)
			continue;

		for (int i = 0; i < heroData[t].size(); i++)
		{
			if (!heroData[t][i]->isDead())
			{
				isEnd = false;
			}
		}

		if (isEnd)
		{
			end((t + 1) % 2);
		}
	}

	return false;
}

void Match::removeEmptyLine()
{
	for (int t = 0; t < 2; t++)
	{
		for (int y = 1; y >= 0; y--)
		{
			bool isEmptyLine = true;
			for (int i = 0; i < 4; i++)
			{
				if (heroData[t][i]->getPos().y == y)
				{
					isEmptyLine = false;
				}
			}

			if (isEmptyLine)
			{
				for (int i = 0; i < 4; i++)
				{
					Point pos = heroData[t][i]->getPos();
					if (pos.y > y)
					{
						heroData[t][i]->setPos({ pos.x, pos.y - 1 });
					}
				}
			}
		}
	}
}

void Match::pickHero(std::shared_ptr<Player> player, int pick1, int pick2)
{
	PickTurn packet;
	packet.type = Type::PICK_TURN;

	PickData data;

	data.type = Type::PICK_DATA;

	switch (pick)
	{
	case 0:
		heroData[0][0] = getHeroFromInfo(player->getHeroInfo(pick1), 0);
		heroIndex[0][0] = pick1;
		packet.pickNum = 2;
		sendPacket(1, packet);

		data.turn = 0;
		data.heroNum = 1;
		data.heroType[0] = heroData[0][0]->getType();
		broadcastPacket(data);
		break;
	case 1:
		heroData[1][0] = getHeroFromInfo(player->getHeroInfo(pick1), 0);
		heroData[1][1] = getHeroFromInfo(player->getHeroInfo(pick2), 1);
		heroIndex[1][0] = pick1;
		heroIndex[1][1] = pick2;
		packet.pickNum = 2;
		sendPacket(0, packet);

		data.turn = 1;
		data.heroNum = 2;
		data.heroType[0] = heroData[1][0]->getType();
		data.heroType[1] = heroData[1][1]->getType();
		broadcastPacket(data);
		break;
	case 2:
		heroData[0][1] = getHeroFromInfo(player->getHeroInfo(pick1), 1);
		heroData[0][2] = getHeroFromInfo(player->getHeroInfo(pick2), 2);
		heroIndex[0][1] = pick1;
		heroIndex[0][2] = pick2;
		packet.pickNum = 2;
		sendPacket(1, packet);

		data.turn = 0;
		data.heroNum = 2;
		data.heroType[0] = heroData[0][1]->getType();
		data.heroType[1] = heroData[0][2]->getType();
		broadcastPacket(data);
		break;
	case 3:
		heroData[1][2] = getHeroFromInfo(player->getHeroInfo(pick1), 2);
		heroData[1][3] = getHeroFromInfo(player->getHeroInfo(pick2), 3);
		heroIndex[1][2] = pick1;
		heroIndex[1][3] = pick2;
		packet.pickNum = 1;
		sendPacket(0, packet);

		data.turn = 1;
		data.heroNum = 2;
		data.heroType[0] = heroData[1][2]->getType();
		data.heroType[1] = heroData[1][3]->getType();
		broadcastPacket(data);
		break;
	case 4:
		heroData[0][3] = getHeroFromInfo(player->getHeroInfo(pick1), 3);
		heroIndex[0][3] = pick1;

		data.turn = 0;
		data.heroNum = 1;
		data.heroType[0] = heroData[0][3]->getType();
		broadcastPacket(data);
		break;
	}

	if (pick == 4)
	{
		PickEnd end;
		end.type = Type::PICK_END;

		for (int t = 0; t < MAX_NUM; t++)
		{
			for (int i = 0; i < 4; i++)
			{
				end.heroIdx[i] = heroIndex[t][i];
			}

			sendPacket(t, end);
		}
		
		state = MatchState::PLACE;
	}
	else
	{
		pick++;
	}
}

void Match::begin()
{
	MatchStart start;

	start.type = Type::MATCH_START;
	start.turn = 0;

	sendPacket(0, start);

	start.turn = 1;
	sendPacket(1, start);

	PickTurn packet;

	packet.type = Type::PICK_TURN;
	packet.pickNum = 1;

	sendPacket(0, packet);
}

void Match::end(int winner)
{
	MatchEnd end;
	end.type = Type::MATCH_END;
	end.winner = winner;

	resetPlayer();

	broadcastPacket(end);

	for (int t = 0; t < MAX_NUM; t++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (heroData[t][i]->isDead())
			{
				players[t]->invalidHeroInfo(i);
			}
		}

		players[t]->matchEnd(t == winner);
	}
}

void Match::surrender(std::shared_ptr<Player> player)
{
	int t = getPlayerIndex(player);

	end((t + 1) % 2);
}

void Match::resetPlayer()
{
	players[0]->resetMatch();
	players[1]->resetMatch();
	GameManager::getInstance()->removePlayerMatchMap(players[0]);
	GameManager::getInstance()->removePlayerMatchMap(players[1]);
}

void Match::broadcastHeroState(int t, int heroIdx, bool isMove)
{
	ChangeHeroState state;

	state.type = Type::CHANGE_HERO_STATE;
	state.turn = t;
	state.idx = heroIdx;
	state.maxHp = heroData[t][heroIdx]->getMaxHp();
	state.hp = heroData[t][heroIdx]->getHp();
	state.maxAct = heroData[t][heroIdx]->getMaxAct();
	state.act = heroData[t][heroIdx]->getAct();
	state.x = heroData[t][heroIdx]->getPos().x;
	state.y = heroData[t][heroIdx]->getPos().y;
	state.isMove = isMove ? 1 : 0;

	//���� hero���� �ɸ� state ���� ����.
	for (auto& it = heroData[t][heroIdx]->states.begin(); it != heroData[t][heroIdx]->states.end();)
	{
		State& state = *it->get();

		if (state.isEnded())
		{
			it = heroData[t][heroIdx]->states.erase(it);
			RemoveHeroState remove;

			remove.type = Type::HERO_REMOVE_STATE;
			remove.stateType = state.getType();
			remove.targetTurn = t;
			remove.targetIdx = heroIdx;
			remove.stateId = state.getId();

			broadcastPacket(remove);
		}
		else
		{
			HeroState s;

			s.type = Type::HERO_STATE;
			s.stateType = state.getType();
			s.act = state.getAct();
			s.attack = state.getAttack();
			s.damaged = state.getDamage();
			s.defence = state.getDefence();
			s.duration = state.getDuration();
			s.executerIdx = state.getExecuter();
			s.executerTurn = state.isMyField() ? t : (t + 1) % 2;
			s.targetIdx = state.getTarget();
			s.targetTurn = t;
			s.hp = state.getHp();
			s.stateId = state.getId();

			broadcastPacket(s);
			++it;
		}
	}

	broadcastPacket(state);
}

void Match::sendReject(int idx)
{
	Reject packet;
	packet.type = Type::REJECT;

	sendPacket(idx, packet);
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
