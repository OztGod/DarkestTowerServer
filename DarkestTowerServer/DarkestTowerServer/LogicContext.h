#pragma once
#include "skylark.h"
#include "Hero.h"

class ClientSession;
class Player;

template<typename Packet>
struct PacketContext : skylark::Context
{
	PacketContext() = default;
	~PacketContext() override = default;

	bool onComplete(int transferred, int key) override
	{
		return session->sendPacket(packet);
	}

	//call when onComplete return false
	bool onFailure() override
	{
		return true;
	}

	Packet packet;
	ClientSession* session;
};

struct AddMatchPendingContext : skylark::Context
{
	AddMatchPendingContext() = default;
	~AddMatchPendingContext() override = default;

	bool onComplete(int transferred, int key) override;

	bool onFailure() override;

	std::shared_ptr<Player> player;
};

struct AllocHeroContext : skylark::Context
{
	AllocHeroContext() = default;
	~AllocHeroContext() override = default;

	bool onComplete(int transferred, int key) override;

	bool onFailure() override;

	std::shared_ptr<Player> player;
	int posNum;
	Point pos[4];
};

struct MoveHeroContext : skylark::Context
{
	MoveHeroContext() = default;
	~MoveHeroContext() override = default;

	bool onComplete(int transferred, int key) override;

	bool onFailure() override;

	std::shared_ptr<Player> player;
	int idx;
	Point pos;
};

struct TurnEndContext : skylark::Context
{
	TurnEndContext() = default;
	~TurnEndContext() override = default;

	bool onComplete(int transferred, int key) override;

	bool onFailure() override;

	std::shared_ptr<Player> player;
};