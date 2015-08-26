#pragma once

class State
{
public:
	State(int damage_, int hp_, int act_, int attack_, int defence_, int duration_)
		:damage(damage_), hp(hp_), act(act_), attack(attack_), defence(defence_), duration(duration_)
	{}

	virtual ~State() = default;

	int getDamage() const { return damage; }
	int getHp() const { return hp; }
	int getAct() const { return act; }
	int getAttack() const { return attack; }
	int getDefence() const { return defence; }
	int getDuration() const { return duration; }
	bool isEnded() const { return ended; }

	virtual void update() = 0;

protected:
	void end();

	int damage;
	int hp;
	int act;
	int attack;
	int defence;
	int duration;
	bool ended = false;
};