#include "Player.h"
#include "Match.h"
#include "HmmoApplication.h"
#include "DBHelper.h"
#include "ClientSession.h"

void Player::init(int win_, int lose_, int elo_, int heroNum_)
{
	win = win_;
	lose = lose_;
	elo = elo_;
	heroNum = heroNum_;

	HmmoApplication::getInstance()->getDBPort()->doLambda([this]()
	{
		DBHelper dbHelper;

		int heroId;
		int heroType;
		int level;
		int exp;
		int maxHp;
		int maxAp;

		dbHelper.bindParamInt(&pid);

		dbHelper.bindResultColumnInt(&heroId);
		dbHelper.bindResultColumnInt(&heroType);
		dbHelper.bindResultColumnInt(&level);
		dbHelper.bindResultColumnInt(&exp);
		dbHelper.bindResultColumnInt(&maxHp);
		dbHelper.bindResultColumnInt(&maxAp);

		if (dbHelper.execute(L"{ call dbo.spLoadPlayerHeros (?) }"))
		{
			while (dbHelper.fetchRow())
			{
				HmmoApplication::getInstance()->getLogicPort()->doLambda([this, heroId, heroType, level, exp, maxHp, maxAp]()
				{
					HeroInfo info;

					info.id = heroId;
					info.type = static_cast<HeroClass>(heroType);
					info.level = level;
					info.exp = exp;
					info.maxHp = maxHp;
					info.maxAct = maxAp;

					heroInfo.push_back(info);
					
					return true;
				});
			}
		}
		
		ClientSession* session = this->getSession();

		return HmmoApplication::getInstance()->getIoPort()->doLambda([this, session]()
		{
			LoginResponse response;
			response.type = Type::LOGIN_RESPONSE;
			response.result = LoginResult::SUCCESS;
			session->sendPacket(response);

			return HmmoApplication::getInstance()->getLogicPort()->doLambda([this, session]()
			{
				enterLobby();

				return true;
			});
		});
	});
}

void Player::enterLobby()
{
	state = PlayerState::LOBBY;

	HmmoApplication::getInstance()->getIoPort()->doLambda([this]()
	{
		EnterLobby packet;

		packet.type = Type::ENTER_LOBBY;

		packet.win = win;
		packet.lose = lose;
		packet.heroNum = heroNum;

		session->sendPacket(packet);

		for (int i = 0; i < heroInfo.size(); i++)
		{
			HeroData data;

			data.type = Type::HERO_DATA;
			data.heroType = heroInfo[i].type;
			data.hp = heroInfo[i].maxHp;
			data.act = heroInfo[i].maxAct;
			data.level = heroInfo[i].level;
			data.skillNum = heroInfo[i].skillType.size();

			for (int j = 0; j < heroInfo[i].skillType.size(); j++)
			{
				data.skillType[j] = static_cast<int>(heroInfo[i].skillType[j]);
				data.skillLevel[j] = 1;
			}

			session->sendPacket(data);
		}

		return true;
	});
}

void Player::matchEnd(bool isWin)
{
	for (int i = 0; i < heroInfo.size(); i++)
	{
		if (!heroInfo[i].isValid)
		{
			heroInfo[i] = makeRandomHeroInfo();
		}
		else
		{
			heroInfo[i].exp += 20;
		}
	}

	//승패 갱신
	if (isWin)
	{
		win = win + 1;
		HmmoApplication::getInstance()->getDBPort()->doLambda([this]()
		{
			DBHelper dbHelper;
			int res;

			dbHelper.bindParamInt(&pid);
			dbHelper.bindResultColumnInt(&res);

			if (dbHelper.execute(L"{ call dbo.spPlayerWin (?) }"))
			{
				dbHelper.fetchRow();
			}
			return true;
		});
	}
	else
	{
		lose = lose + 1;
		HmmoApplication::getInstance()->getDBPort()->doLambda([this]()
		{
			DBHelper dbHelper;
			int res;

			dbHelper.bindParamInt(&pid);
			dbHelper.bindResultColumnInt(&res);

			if (dbHelper.execute(L"{ call dbo.spPlayerLose (?) }"))
			{
				dbHelper.fetchRow();
			}
			return true;
		});
	}

	HmmoApplication::getInstance()->getDBPort()->doLambda([this]()
	{
		for (int i = 0; i < heroInfo.size(); i++)
		{
			DBHelper dbHelper;
			int res;

			dbHelper.bindParamInt(&heroInfo[i].id);
			dbHelper.bindParamInt(&heroInfo[i].level);
			dbHelper.bindParamInt(&heroInfo[i].exp);
			dbHelper.bindParamInt(&heroInfo[i].maxHp);
			dbHelper.bindParamInt(&heroInfo[i].maxAct);

			dbHelper.bindResultColumnInt(&res);

			if (dbHelper.execute(L"{ call dbo.spUpdatePlayerHeros (?, ?, ?, ?, ?) }"))
			{
				dbHelper.fetchRow();
			}

			//데이터 갱신후 다시 로비로
			HmmoApplication::getInstance()->getLogicPort()->doLambda([this]()
			{
				enterLobby();
				return true;
			});
		}

		return true;
	});
}
