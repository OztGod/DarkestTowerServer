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
};
enum class Type : uint8_t
{
	LOGIN_REQUEST = 0,
	LOGIN_RESPONSE = 1,
	ALLOC_HERO = 2,
	RANDOM_HERO_REQUEST = 3,
	RANDOM_HERO_RESPONSE = 4,
	MATCH_START = 5,
	GAME_DATA = 6,
	MOVE_HERO = 7,
	CHANGE_HERO_STATE = 8,
	TURN_END = 9,
	UPDATE_TURN = 10,
	TYPE_NUM = 11,
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
struct RandomHeroRequest : Header
{
};
#pragma pack(pop)
#pragma pack(push, 1)
struct RandomHeroResponse : Header
{
	HeroClass heroClass[4];
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
	int8_t skillNum[4];
	int8_t skillIdx[4];
	int8_t skillLevel[4];
	int8_t x[4];
	int8_t y[4];
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
	int8_t idx;
	int8_t hp;
	int8_t act;
	int8_t x;
	int8_t y;
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
