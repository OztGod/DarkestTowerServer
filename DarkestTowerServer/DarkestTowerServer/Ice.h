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

	//해당 상태에 걸린 캐릭터가 공격하거나 공격 당할 때마다 발동
	void apply(Hero* hero, Hero* enemy, bool isAct) override;

	//해당 상태에 걸린 캐릭터의 턴이 돌아올 때마다 발동
	void update(Hero* hero) override;

	void effectUpdate(std::vector<int> effect, UHeroVec& heros, bool isMyTurn) override;
};