#include <locale>
#include <codecvt>
#include "GameManager.h"
#include "Player.h"
#include "ClientSession.h"
#include "LogicContext.h"
#include "HmmoApplication.h"
#include "Match.h"
#include "DBHelper.h"

GameManager* GameManager::instance = nullptr;

GameManager * GameManager::getInstance()
{
	if (instance == nullptr)
		instance = new GameManager();
	
	return instance;
}

void GameManager::isValidAccount(const char * id, int idLength, const char * password, int pwdLength, AccountFunc complete)
{
	std::string idStr(id, id + idLength);
	std::string passwordStr(password, password + pwdLength);
	HmmoApplication::getInstance()->getDBPort()->doLambda([id = idStr, password = passwordStr, complete]()
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		
		std::wstring wid = converter.from_bytes(id);
		std::wstring wpassword = converter.from_bytes(password);

		DBHelper dbHelper;
		int pid = 0;
		int win = 0;
		int lose = 0;
		int elo = 0;
		int championNum = 0;
		bool isValid = false;
		wchar_t comment[256] = { 0, };

		dbHelper.bindParamText(wid.c_str());
		dbHelper.bindParamText(wpassword.c_str());

		dbHelper.bindResultColumnInt(&pid);
		dbHelper.bindResultColumnInt(&win);
		dbHelper.bindResultColumnInt(&lose);
		dbHelper.bindResultColumnInt(&elo);
		dbHelper.bindResultColumnInt(&championNum);
		dbHelper.bindResultColumnBool(&isValid);
		dbHelper.bindResultColumnText(comment, 256);

		if (dbHelper.execute(L"{ call dbo.spLoadPlayer (? , ?) }"))
		{
			if (dbHelper.fetchRow())
			{
				HmmoApplication::getInstance()->getIoPort()->doLambda([pid, win, lose, elo, championNum, complete]()
				{
					complete(pid, win, lose, elo, championNum);
					return true;
				});
			}
			else
			{
				HmmoApplication::getInstance()->getIoPort()->doLambda([pid, complete]()
				{
					complete(-1, 0, 0, 0, 0);
					return true;
				});
			}
		}

		return true;
	});
}

void GameManager::addMatchPendingList(std::shared_ptr<Player> player)
{
	matchPendingList.push_back(player);
}

void GameManager::removeMatchPendingList(std::shared_ptr<Player> player)
{
	matchPendingList.remove(player);
}

void GameManager::login(int pid)
{
	printf("login! id = %d\n", pid);
	HmmoApplication::getInstance()->getDBPort()->doLambda([pid]()
	{
		DBHelper dbHelper;
		int id = pid;
		int res = 0;
		bool valid = false;

		dbHelper.bindParamInt(&id);
		dbHelper.bindParamBool(&valid);
		dbHelper.bindResultColumnInt(&res);

		if (dbHelper.execute(L"{ call dbo.spUpdatePlayerValid (? , ?) }"))
		{
			return dbHelper.fetchRow();
		}

		return false;
	});
}

void GameManager::logout(int pid)
{
	printf("logout! id = %d\n", pid);
	HmmoApplication::getInstance()->getDBPort()->doLambda([pid]()
	{
		DBHelper dbHelper;
		int id = pid;
		int res = 0;
		bool valid = true;

		dbHelper.bindParamInt(&id);
		dbHelper.bindParamBool(&valid);
		dbHelper.bindResultColumnInt(&res);

		if (dbHelper.execute(L"{ call dbo.spUpdatePlayerValid (? , ?) }"))
		{
			return dbHelper.fetchRow();
		}
		return false;
	});
}

void GameManager::update()
{
	createMatch();
	updateMatch();
}

void GameManager::createMatch()
{
	//match 대기중인 player가 2명 이상이면 경기를 하나 만든다
	while (matchPendingList.size() >= 2)
	{
		auto player = matchPendingList.front();
		matchPendingList.pop_front();

		if (!player->getSession()->isConnected() || player->getMatch() != nullptr)
		{
			continue;
		}

		auto player2 = matchPendingList.front();
		matchPendingList.pop_front();

		if (!player2->getSession()->isConnected() || player2->getMatch() != nullptr)
		{
			matchPendingList.push_front(player);
			continue;
		}

		Match* newMatch = new Match();
		newMatch->registerPlayer(player);
		newMatch->registerPlayer(player2);
		newMatch->begin();

		matchList.push_back(newMatch);
		playerMatchMap[player->getId()] = newMatch;
		playerMatchMap[player2->getId()] = newMatch;
	}
}

void GameManager::updateMatch()
{
	for (auto iter = matchList.begin(); iter != matchList.end();)
	{
		Match* match = *iter;

		if (match->isEnd())
		{
			printf("match end\n");
			delete match;
			iter = matchList.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	for (auto iter = matchPendingList.begin(); iter != matchPendingList.end();)
	{
		auto& player = *iter;

		if (!player->getSession()->isConnected())
		{
			iter = matchPendingList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void GameManager::removePlayerMatchMap(std::shared_ptr<Player> player)
{
	playerMatchMap.erase(player->getId());
}

void GameManager::initAccount(int pid)
{
	for (int i = 0; i < baseHeroNum; i++)
	{
		HeroInfo info = makeRandomHeroInfo();

		int hid;
		
		{
			int t = static_cast<int>(info.type);

			DBHelper dbHelper;

			dbHelper.bindParamInt(&pid);
			dbHelper.bindParamInt(&t);
			dbHelper.bindParamInt(&info.level);
			dbHelper.bindParamInt(&info.maxHp);
			dbHelper.bindParamInt(&info.maxAct);
			dbHelper.bindParamInt(&info.hpGrow);
			dbHelper.bindParamInt(&info.actGrow);

			dbHelper.bindResultColumnInt(&hid);

			dbHelper.execute(L"{ call dbo.spInsertPlayerHeros (?, ?, ?, ?, ?, ?, ?) }");
			dbHelper.fetchRow();
		}

		for (int i = 0; i < info.skillType.size(); i++)
		{
			DBHelper skillDB;
			int res;
			int skillType = static_cast<int>(info.skillType[i]);

			skillDB.bindParamInt(&hid);
			skillDB.bindParamInt(&skillType);

			skillDB.bindResultColumnInt(&res);

			skillDB.execute(L"{ call dbo.spInsertHeroSkills (?, ?) }");
			skillDB.fetchRow();
		}
	}
}

GameManager::GameManager()
{
}
