#pragma once
#include "Skylark.h"
#include "pcPacket.h"

class Player;

class ClientSession : public skylark::Session, public skylark::PacketHandler<Header, ClientSession, static_cast<int>(Type::TYPE_NUM)>
{
public:
	ClientSession(skylark::CompletionPort* port, std::size_t sendBufSize, std::size_t recvBufSize);
	~ClientSession() override;

	int select(Header header) override;
	
	static void initHandler();

	bool onAccept() override;
	bool onDisconnect(int reason) override;

	bool onRead() override;
	
	void onRelease() override;

	void sessionReset();

	void onLoginRequest(const LoginRequest& packet);
	void onRandomHeroRequest(const RandomHeroRequest& packet);
	void onAllocHero(const AllocHero& packet);
	void onMoveHero(const MoveHero& packet);
	void onTurnEnd(const TurnEnd& packet);
	void onSelectHero(const SelectHero& packet);
	void onSkillRangeRequest(const SkillRangeRequest& packet);
	void onActHero(const ActHero& packet);
	void onRegisterAccount(const RegisterAccountRequest& packet);

private:
	std::shared_ptr<Player> player = nullptr;
};