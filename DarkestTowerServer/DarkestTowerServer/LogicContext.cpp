#include "LogicContext.h"
#include "GameManager.h"
#include "Player.h"
#include "Match.h"

bool AddMatchPendingContext::onComplete(int transferred, int key)
{
	GameManager::getInstance()->addMatchPendingList(player);
	return true;
}

bool AddMatchPendingContext::onFailure()
{
	return true;
}

bool AllocHeroContext::onComplete(int transferred, int key)
{
	player->getMatch()->placeHero(player, posNum, pos);
	return true;
}

bool AllocHeroContext::onFailure()
{
	return true;
}

bool MoveHeroContext::onComplete(int transferred, int key)
{
	player->getMatch()->moveHero(player, idx, pos);
	return false;
}

bool MoveHeroContext::onFailure()
{
	return true;
}

bool TurnEndContext::onComplete(int transferred, int key)
{
	player->getMatch()->turnChange(player);
	return true;
}

bool TurnEndContext::onFailure()
{
	return true;
}

bool SelectHeroContext::onComplete(int transferred, int key)
{
	player->getMatch()->selectHero(player, idx);
	return true;
}

bool SelectHeroContext::onFailure()
{
	return true;
}

bool SkillRangeContext::onComplete(int transferred, int key)
{
	player->getMatch()->getSkillRange(player, heroIdx, skillIdx);
	return true;
}

bool SkillRangeContext::onFailure()
{
	return true;
}

bool ActHeroContext::onComplete(int transferred, int key)
{
	player->getMatch()->actHero(player, heroIdx, skillIdx, pos);
	return true;
}

bool ActHeroContext::onFailure()
{
	return true;
}
