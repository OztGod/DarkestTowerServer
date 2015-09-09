#pragma once
#include <vector>
#include "Types.h"
#include "pcPacket.h"

class Hero;

class State
{
public:
	State(StateType type_, int executer_, bool executerMyField_, int target_, int damage_, int hp_, int act_, int attack_, int defence_, int duration_)
		:type(type_), damage(damage_), hp(hp_), act(act_), attack(attack_), defence(defence_), duration(duration_), executer(executer_), executerMyField(executerMyField_), target(target_)
	{
		static int id_ = 0;

		id = id_;

		id_++;
	}

	virtual ~State() = default;

	int getDamage() const { return damage; }
	int getHp() const { return hp; }
	int getAct() const { return act; }
	int getAttack() const { return attack; }
	int getDefence() const { return defence; }
	int getDuration() const { return duration; }
	int getExecuter() const { return executer; }
	bool isMyField() const { return executerMyField; }
	int getTarget() const { return target; }
	bool isEnded() const { return ended; }
	int getId() const { return id; }
	StateType getType() const { return type; }

	//�ش� ���¿� �ɸ� ĳ���Ͱ� �����ϰų� ���� ���� ������ �ߵ�
	virtual void apply(Hero* hero, bool isAct) = 0;

	//�ش� ���¿� �ɸ� ĳ������ ���� ���ƿ� ������ �ߵ�
	virtual void update(Hero* hero) = 0;

	//��ų ���� �� ����� �� ���
	virtual void effectUpdate(std::vector<int> effect, UHeroVec& heros, bool isMyTurn) = 0;

protected:
	void end();

	StateType type;

	int id;
	int damage;
	int hp;
	int act;
	int attack;
	int defence;
	int duration;

	int executer;
	bool executerMyField;
	int target;

	bool ended = false;
};