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

		return HmmoApplication::getInstance()->getIoPort()->doLambda([session]()
		{
			LoginResponse response;
			response.type = Type::LOGIN_RESPONSE;
			response.result = LoginResult::SUCCESS;
			return session->sendPacket(response);
		});
	});
}
