#include "LogicContext.h"
#include "GameManager.h"

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
	GameManager::getInstance()->placeHero(player, posNum, pos);
	return true;
}

bool AllocHeroContext::onFailure()
{
	return true;
}
