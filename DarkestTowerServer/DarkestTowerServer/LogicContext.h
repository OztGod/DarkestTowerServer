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