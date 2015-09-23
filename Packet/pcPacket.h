#pragma once
#include <stdint.h>

enum class HeroClass : uint8_t
{
	FIGHTER = 0,
	MAGICIAN = 1,
	ARCHER = 2,
	THIEF = 3,
	PRIEST = 4,
	MONK = 5,
	NUM = 6,
};
enum class Type : uint8_t
{
	LOGIN_REQUEST = 0,
	LOGIN_RESPONSE = 1,
	ALLOC_HERO = 2,
	MATCH_START = 3,
	MATCH_END = 4,
	GAME_DATA = 5,
	SKILL_DATA = 6,
	CHANGE_HERO_STATE = 7,
	SELECT_HERO = 8,
	VALID_SKILLS = 9,
	SKILL_RANGE_REQUEST = 10,
	SKILL_RANGE_RESPONSE = 11,
	SKILL_SHOT = 12,
	MOVE_HERO = 13,
	ACT_HERO = 14,
	DEAD_HERO = 15,
	TURN_END = 16,
	UPDATE_TURN = 17,
	REJECT = 18,
	HERO_STATE = 19,
	HERO_REMOVE_STATE = 20,
	EFFECT_RESPONSE = 21,
	REGISTER_ACCOUNT_REQUEST = 22,
	REGISTER_ACCOUNT_RESPONSE = 23,
	REQUEST_MATCH = 24,
	CANCEL_MATCH = 25,
	ENTER_LOBBY = 26,
	HERO_DATA = 27,
	PICK_TURN = 28,
	PICK = 29,
	PICK_DATA = 30,
	PICK_END = 31,
	SURRENDER = 32,
	TYPE_NUM = 33,
};
enum class StateType : uint8_t
{
	STATE_IRON = 0,
	STATE_POISON = 1,
	STATE_ICE = 2,
	STATE_BURN = 3,
	STATE_BUFF = 4,
	STATE_TAUNT = 5,
	STATE_SACRIFICE = 6,
	STATE_PRAY = 7,
};
enum class SkillType : uint8_t
{
	FIGHTER_ATTACK = 0,
	FIGHTER_CHARGE = 1,
	FIGHTER_HARD = 2,
	FIGHTER_IRON = 3,
	MAGICIAN_ICE_ARROW = 4,
	MAGICIAN_FIRE_BLAST = 5,
	MAGICIAN_THUNDER_STORM = 6,
	MAGICIAN_POLYMORPH = 7,
	ARCHER_ATTACK = 8,
	ARCHER_BACK_ATTACK = 9,
	ARCHER_PENETRATE_SHOT = 10,
	ARCHER_SNIPE = 11,
	THIEF_ATTACK = 12,
	THIEF_BACK_STEP = 13,
	THIEF_POISON = 14,
	THIEF_TAUNT = 15,
	PRIEST_HEAL = 16,
	PRIEST_ATTACK = 17,
	PRIEST_BUFF = 18,
	PRIEST_REMOVE_MAGIC = 19,
	MONK_ATTACK = 20,
	MONK_SACRIFICE = 21,
	MONK_PRAY = 22,
	MONK_KICK = 23,
};
enum class LoginResult : uint8_t
{
	FAILED = 0,
	SUCCESS = 1,
};
#pragma pack(push, 1)
struct Header
{
	Type type;
	char foo;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct LoginRequest : Header
{
	int8_t idLength;
	int8_t passwordLength;
	char id[16];
	char password[16];
};
#pragma pack(pop)
#pragma pack(push, 1)
struct LoginResponse : Header
{
	LoginResult result;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct AllocHero : Header
{
	int8_t allocNum;
	int8_t x[4];
	int8_t y[4];
};
#pragma pack(pop)
#pragma pack(push, 1)
struct MatchStart : Header
{
	int8_t turn;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct GameData : Header
{
	int8_t turn;
	int8_t classNum;
	HeroClass classes[4];
	int8_t hp[4];
	int8_t act[4];
	int8_t x[4];
	int8_t y[4];
};
#pragma pack(pop)
#pragma pack(push, 1)
struct SkillData : Header
{
	int8_t turn;
	int8_t heroIdx;
	int8_t skillNum;
	SkillType skillType[4];
	int8_t skillLevel[4];
};
#pragma pack(pop)
#pragma pack(push, 1)
struct MoveHero : Header
{
	int8_t idx;
	int8_t x;
	int8_t y;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct ChangeHeroState : Header
{
	int8_t turn;
	int8_t idx;
	int8_t maxHp;
	int8_t hp;
	int8_t maxAct;
	int8_t act;
	int8_t x;
	int8_t y;
	int8_t isMove;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct TurnEnd : Header
{
	int8_t turn;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct UpdateTurn : Header
{
	int8_t nowTurn;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct SelectHero : Header
{
	int8_t idx;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct ValidSkills : Header
{
	int8_t num;
	int8_t idx[6];
};
#pragma pack(pop)
#pragma pack(push, 1)
struct SkillRangeRequest : Header
{
	int8_t heroIdx;
	int8_t skillIdx;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct SkillRangeResponse : Header
{
	int8_t heroIdx;
	int8_t skillIdx;
	int8_t isMyField;
	int8_t rangeNum;
	int8_t rangeX[9];
	int8_t rangeY[9];
};
#pragma pack(pop)
#pragma pack(push, 1)
struct EffectResponse : Header
{
	int8_t heroIdx;
	int8_t skillIdx;
	int8_t effectNum;
	int8_t effectX[9];
	int8_t effectY[9];
};
#pragma pack(pop)
#pragma pack(push, 1)
struct ActHero : Header
{
	int8_t heroIdx;
	int8_t skillIdx;
	int8_t x;
	int8_t y;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct SkillShot : Header
{
	int8_t turn;
	int8_t heroIdx;
	int8_t skillIdx;
	int8_t effectNum;
	int8_t effectTurn[8];
	int8_t effectX[8];
	int8_t effectY[8];
};
#pragma pack(pop)
#pragma pack(push, 1)
struct MatchEnd : Header
{
	int8_t winner;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct Reject : Header
{
};
#pragma pack(pop)
#pragma pack(push, 1)
struct DeadHero : Header
{
	int8_t turn;
	int8_t heroIdx;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct HeroState : Header
{
	StateType stateType;
	int8_t targetTurn;
	int8_t targetIdx;
	int8_t executerTurn;
	int8_t executerIdx;
	int8_t stateId;
	int8_t damaged;
	int8_t hp;
	int8_t act;
	int8_t attack;
	int8_t defence;
	int8_t duration;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct RemoveHeroState : Header
{
	int8_t targetTurn;
	int8_t targetIdx;
	int8_t stateId;
	StateType stateType;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct RegisterAccountRequest : Header
{
	int8_t idLength;
	char id[16];
	int8_t passwordLength;
	char password[16];
};
#pragma pack(pop)
#pragma pack(push, 1)
struct RegisterAccountResponse : Header
{
	int8_t isSuccess;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct RequestMatch : Header
{
};
#pragma pack(pop)
#pragma pack(push, 1)
struct CancelMatch : Header
{
};
#pragma pack(pop)
#pragma pack(push, 1)
struct EnterLobby : Header
{
	int32_t win;
	int32_t lose;
	int8_t heroNum;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct HeroData : Header
{
	HeroClass heroType;
	int8_t level;
	int8_t hp;
	int8_t act;
	int8_t skillNum;
	int8_t skillType[4];
	int8_t skillLevel[4];
};
#pragma pack(pop)
#pragma pack(push, 1)
struct PickTurn : Header
{
	int8_t pickNum;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct Pick : Header
{
	int8_t heroIdx[2];
};
#pragma pack(pop)
#pragma pack(push, 1)
struct PickData : Header
{
	int8_t turn;
	int8_t heroNum;
	HeroClass heroType[2];
};
#pragma pack(pop)
#pragma pack(push, 1)
struct PickEnd : Header
{
	int8_t heroIdx[4];
};
#pragma pack(pop)
#pragma pack(push, 1)
struct Surrender : Header
{
};
#pragma pack(pop)
