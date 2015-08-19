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
	int t = getPlayerIndex(player);

	//이미 겜 시작한 상태면 무시
	if (isStart)
	{
		sendReject(t);
		return;
	}

	isReady[t] = true;

	//둘 다 ready(영웅 배치 제출)면 진짜 게임 시작(게임 시작 패킷 전송)
	if (isAllReady())
	{
		printf("send gameData...\n");
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

		UpdateTurn packet;
		
		packet.type = Type::UPDATE_TURN;
		packet.nowTurn = nowTurn;

		broadcastPacket(packet);
	}
}

void Match::placeHero(std::shared_ptr<Player>& player, int num, Point * points)
{
	int t = getPlayerIndex(player);

	//히어로 위치 배정은 겜 시작하기 전에만 가능(레디 박기 전에만 가능)
	if (isStart || isReady[t])
	{
		sendReject(t);
		return;
	}

	for (int i = 0; i < num; i++)
	{
		heroData[t][i]->setPos(points[i]);
	}

	ready(player);
}

void Match::moveHero(std::shared_ptr<Player>& player, int idx, Point pos)
{
	//겜 시작했을 때만 이동 가능
	int t = getPlayerIndex(player);

	if (!isStart)
	{
		sendReject(t);
		return;
	}

	//자기 턴도 아닌데 움직일려고 하면 거부
	if (t != nowTurn)
		return;

	int swapIdx = -1;

	for (int i = 0; i < heroData[t].size(); i++)
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
	{
		sendReject(t);
		return;
	}

	Point prevPos = heroData[t][idx]->getPos();

	//pos 바꾸는 위치가 이상하면 거부
	if (!heroData[t][idx]->move(pos))
	{
		sendReject(t);
		return;
	}

	map.move(prevPos, pos, t);

	if (swapIdx != -1)
	{		
		heroData[t][swapIdx]->setPos(prevPos);

		//자리 바뀐 애도 바뀌었다는 패킷 보내주기
		sendHeroState(t, swapIdx);
	}

	sendHeroState(t, idx);
}

void Match::randomHero(std::shared_ptr<Player>& player)
{
	int t = getPlayerIndex(player);

	//겜시작 or 레디 박은 상태면 히어로 변경 불가
	if (isStart || isReady[t])
	{
		sendReject(t);
		return;
	}
	
	heroData[t].clear();

	for (int i = 0; i < 4; i++)
	{
		heroData[t].push_back(getRandomHero());
	}
}

void Match::getHeroData(std::shared_ptr<Player>& player, OUT std::vector<const Hero*>& data)
{
	int t = getPlayerIndex(player);

	for (int i = 0; i < heroData[t].size(); i++)
	{
		data.push_back(heroData[t][i].get());
	}
}

void Match::selectHero(std::shared_ptr<Player>& player, int idx)
{
	//해당 플레이어의 idx번째 영웅에 대해 해당 위치에서 사용 가능한 모든 스킬을 돌려준다
	int t = getPlayerIndex(player);
	std::vector<int> validSkills;

	printf("[player %d] : select hero..", t);

	auto heroPos = heroData[t][idx]->getPos();

	for (int i = 0; i < heroData[t][idx]->getSkillNum(); i++)
	{
		auto skill = heroData[t][idx]->getSkill(i);

		if (skill->isValidActPos(heroPos, heroData[t], heroData[(t + 1) % 2]) && heroData[t][idx]->getSkillCool(i) == 0)
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

	//skill 쓸 수 있는 위치 아니면 무시
	if (!skill->isActEnable(heroPos, heroData[t], heroData[(t + 1) % 2]))
	{
		sendReject(t);
		return;
	}

	auto range = skill->getRange(heroPos, heroData[t], heroData[(t + 1) % 2]);
	auto effect = skill->getEffectRange(heroPos, heroData[t], heroData[(t + 1) % 2]);

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

	//겜이 시작 안했거나, 자기 턴도 아니면서 턴 바꿀려고 한 경우 무시
	if (!isStart || nowTurn != t)
	{
		sendReject(t);
		return;
	}

	nowTurn = (nowTurn + 1) % playerNum;

	for (int i = 0; i < heroData[t].size(); i++)
	{
		heroData[nowTurn][i]->turnUpdate();
		sendHeroState(t, i);
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

	//skill 사용에 필요한 act 포인트 남아있지 않은 경우 무시
	if (skill->getAct() > heroData[t][heroIdx]->getAct())
	{
		sendReject(t);
		return;
	}

	//skill이 아직 cool인 경우 무시
	if (heroData[t][heroIdx]->getSkillCool(skillIdx) > 0)
	{
		sendReject(t);
		return;
	}

	//skill 사용 불가능한 위치일 경우 무시
	if (!skill->isActEnable(heroPos, heroData[t], heroData[(t + 1) % 2]))
	{
		sendReject(t);
		return;
	}

	auto heroList = skill->getHeroInEffect(pos, heroData[t], heroData[(t + 1) % 2]);
			
	//skill 효과 범위에 들어가는 캐릭터가 하나도 없을 경우 무시
	if (heroList.size() == 0)
	{
		sendReject(t);
		return;
	}

	//스킬 사용했으므로 사용한 놈 act 감소시키고 스킬 쿨 돌게 만든다
	heroData[t][heroIdx]->setSkillCool(skillIdx, skill->getCool());
	heroData[t][heroIdx]->consumeAct(skill->getAct());

	EnemySkillShot packet;
	packet.type = Type::ENEMY_SKILL_SHOT;
	packet.heroIdx = heroIdx;
	packet.skillIdx = skillIdx;

	//상대에게 스킬 사용했음을 나타내는 패킷 보냄
	sendPacket((t + 1) % 2, packet);

	sendHeroState(t, heroIdx);

	for (auto& target : heroList)
	{
		int turn = skill->myField() ? turn : (turn + 1) % 2;
		skill->doSkill(pos, heroData[t][heroIdx].get(), heroData[turn][target].get(), heroData[t], heroData[(t + 1) % 2]);
		//스킬 효과 받았으므로 갱신된 hero 상태 돌려준다
		sendHeroState(turn, target);
	}
}

bool Match::isEnd()
{
	//연결 끊기면 나머지 한쪽이 자동으로 부전승을 거둔다

	if (!players[0]->getSession()->isConnected())
	{
		MatchEnd end;
		end.type = Type::MATCH_END;
		end.winner = 1;

		resetPlayer();

		broadcastPacket(end);

		return true;
	}

	if (!players[1]->getSession()->isConnected())
	{
		MatchEnd end;
		end.type = Type::MATCH_END;
		end.winner = 0;

		resetPlayer();
		
		broadcastPacket(end);

		return true;
	}

	return false;
}

void Match::resetPlayer()
{
	players[0]->resetMatch();
	players[1]->resetMatch();
	GameManager::getInstance()->removePlayerMatchMap(players[0]);
	GameManager::getInstance()->removePlayerMatchMap(players[1]);
}

void Match::sendHeroState(int t, int heroIdx)
{
	ChangeHeroState state;

	state.type = Type::CHANGE_HERO_STATE;
	state.turn = t;
	state.idx = heroIdx;
	state.hp = heroData[t][heroIdx]->getHp();
	state.act = heroData[t][heroIdx]->getAct();
	state.x = heroData[t][heroIdx]->getPos().x;
	state.y = heroData[t][heroIdx]->getPos().y;

	broadcastPacket(state);
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
