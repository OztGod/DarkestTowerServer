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
		std::vector<HeroInfo> infos;

		int heroId;
		int heroType;
		int level;
		int exp;
		int maxHp;
		int maxAp;
		int hpGrow;
		int actGrow;

		{
			DBHelper dbHelper;

			dbHelper.bindParamInt(&pid);

			dbHelper.bindResultColumnInt(&heroId);
			dbHelper.bindResultColumnInt(&heroType);
			dbHelper.bindResultColumnInt(&level);
			dbHelper.bindResultColumnInt(&exp);
			dbHelper.bindResultColumnInt(&maxHp);
			dbHelper.bindResultColumnInt(&maxAp);
			dbHelper.bindResultColumnInt(&hpGrow);
			dbHelper.bindResultColumnInt(&actGrow);

			if (dbHelper.execute(L"{ call dbo.spLoadPlayerHeros (?) }"))
			{
				while (dbHelper.fetchRow())
				{
					HeroInfo info;

					info.id = heroId;
					info.type = static_cast<HeroClass>(heroType);
					info.level = level;
					info.exp = exp;
					info.maxHp = maxHp;
					info.maxAct = maxAp;
					info.hpGrow = hpGrow;
					info.actGrow = actGrow;

					infos.push_back(info);
				}
			}
		}


		for (int i = 0; i < infos.size(); i++)
		{
			std::vector<SkillType> types;

			DBHelper skillDB;
			int id = infos[i].id;
			int skillType;
			int skillLevel;

			skillDB.bindParamInt(&id);

			skillDB.bindResultColumnInt(&skillType);
			skillDB.bindResultColumnInt(&skillLevel);

			//level은 일단 무시

			if (skillDB.execute(L"{ call dbo.spLoadHeroSkills (?) }"))
			{
				while (skillDB.fetchRow())
				{
					types.push_back(static_cast<SkillType>(skillType));
				}
			}

			infos[i].skillType = types;
		}

		HmmoApplication::getInstance()->getLogicPort()->doLambda([this, infos]()
		{
			heroInfo = infos;

			return true;
		});
		
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
	printf("player %d enter lobby\n", pid);
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

void Player::matchEnd(bool isWin, const std::vector<int>& participantHeros)
{
	for (int i = 0; i < participantHeros.size(); i++)
	{
		if (!heroInfo[participantHeros[i]].isValid)
		{
			heroInfo[participantHeros[i]] = makeRandomHeroInfo();
		}
		else
		{
			//경험치 기본적으로 10 ~ 30사이 랜덤.
			heroInfo[participantHeros[i]].exp += 10 + (rand() % 21);

			//이겼을 경우 20 ~ 40 더 줌

			heroInfo[participantHeros[i]].exp += 20 + (rand() % 21);

			//조건 만족하면 levelup 처리함
			heroInfo[participantHeros[i]].levelup();
		}
	}

	//승패 갱신
	if (isWin)
	{
		win = win + 1;
		HmmoApplication::getInstance()->getDBPort()->doLambda([pid = pid]()
		{
			DBHelper dbHelper;
			int id = pid;
			int res;

			dbHelper.bindParamInt(&id);
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
		HmmoApplication::getInstance()->getDBPort()->doLambda([pid = pid]()
		{
			DBHelper dbHelper;
			int id = pid;
			int res;

			dbHelper.bindParamInt(&id);
			dbHelper.bindResultColumnInt(&res);

			if (dbHelper.execute(L"{ call dbo.spPlayerLose (?) }"))
			{
				dbHelper.fetchRow();
			}
			return true;
		});
	}

	HmmoApplication::getInstance()->getDBPort()->doLambda([info = heroInfo, pid = pid]()
	{
		auto heroInfo = info;
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
		}

		//데이터 갱신후 다시 로비로
		HmmoApplication::getInstance()->getLogicPort()->doLambda([pid]()
		{
			auto player = GameManager::getInstance()->getPlayer(pid);

			if (player != nullptr)
			{
				player->enterLobby();
				return true;
			}

			return false;
		});

		return true;
	});
}
