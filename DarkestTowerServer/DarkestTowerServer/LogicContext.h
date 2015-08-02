#pragma once
#include "skylark.h"

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