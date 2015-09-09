#pragma once
#include "State.h"

class Hero;

class Burn : public State
{
public:
	Burn(int executer, bool myField, int target, int damage_, int duration_)
		:State(StateType::STATE_BURN, executer, myField, target, damage_, 0,0 , 0, 0, duration_)
	{}

	~Burn() override = default;

	//�ش� ���¿� �ɸ� ĳ���Ͱ� �����ϰų� ���� ���� ������ �ߵ�
	void apply(Hero* hero, Hero* enemy, bool isAct) override;

	//�ش� ���¿� �ɸ� ĳ������ ���� ���ƿ� ������ �ߵ�
	void update(Hero* hero) override;

	void effectUpdate(std::vector<int> effect, UHeroVec& heros, bool isMyTurn) override;
};