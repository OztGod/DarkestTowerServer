#pragma once
#include "State.h"

class Hero;

class Ice : public State
{
public:
	Ice(int executer, bool myField, int target, int act, int duration_)
		:State(StateType::STATE_ICE, executer, myField, target, 0, 0, act, 0, 0, duration_)
	{}

	~Ice() override = default;

	//�ش� ���¿� �ɸ� ĳ���Ͱ� �����ϰų� ���� ���� ������ �ߵ�
	void apply(Hero* hero, Hero* enemy, bool isAct) override;

	//�ش� ���¿� �ɸ� ĳ������ ���� ���ƿ� ������ �ߵ�
	void update(Hero* hero) override;

	void effectUpdate(std::vector<int> effect, UHeroVec& heros, bool isMyTurn) override;
};