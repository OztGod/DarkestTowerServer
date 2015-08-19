using System;
using System.Text;
using System.Runtime.InteropServices;

namespace Packet
{

public enum HeroClass
{
	FIGHTER = 0,
	MAGICIAN = 1,
	ARCHER = 2,
	THIEF = 3,
	PRIEST = 4,
	MONK = 5,
	NUM = 6,
}
public enum Type
{
	LOGIN_REQUEST = 0,
	LOGIN_RESPONSE = 1,
	ALLOC_HERO = 2,
	RANDOM_HERO_REQUEST = 3,
	RANDOM_HERO_RESPONSE = 4,
	MATCH_START = 5,
	GAME_DATA = 6,
	CHANGE_HERO_STATE = 7,
	SELECT_HERO = 8,
	VALID_SKILLS = 9,
	SKILL_RANGE_REQUEST = 10,
	SKILL_RANGE_RESPONSE = 11,
	ENEMY_SKILL_SHOT = 12,
	MOVE_HERO = 13,
	ACT_HERO = 14,
	TURN_END = 15,
	UPDATE_TURN = 16,
	TYPE_NUM = 17,
}
public enum SkillType
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
}
public enum LoginResult
{
	FAILED = 0,
	SUCCESS = 1,
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class Header
{
	[MarshalAs(UnmanagedType.U1)]
	public byte type;
	[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 1)]
	public string foo;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class LoginRequest : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte idLength;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte passwordLength;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
	public char[] id;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
	public char[] password;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class LoginResponse : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public byte result;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class AllocHero : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte allocNum;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
	public sbyte[] x= new sbyte[4];
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
	public sbyte[] y= new sbyte[4];
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class RandomHeroRequest : Header
{
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class RandomHeroResponse : Header
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
	public byte[] heroClass= new byte[4];
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class MatchStart : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte turn;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class GameData : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte turn;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte classNum;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
	public byte[] classes= new byte[4];
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
	public sbyte[] hp= new sbyte[4];
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
	public sbyte[] act= new sbyte[4];
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
	public sbyte[] skillNum= new sbyte[4];
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
	public byte[] skillType= new byte[4];
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
	public sbyte[] skillLevel= new sbyte[4];
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
	public sbyte[] x= new sbyte[4];
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
	public sbyte[] y= new sbyte[4];
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class MoveHero : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte idx;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte x;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte y;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class ChangeHeroState : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte turn;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte idx;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte hp;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte act;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte x;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte y;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class TurnEnd : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte turn;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class UpdateTurn : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte nowTurn;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class SelectHero : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte idx;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class ValidSkills : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte num;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
	public sbyte[] idx= new sbyte[6];
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class SkillRangeRequest : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte heroIdx;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte skillIdx;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class SkillRangeResponse : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte isMyField;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte rangeNum;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 9)]
	public sbyte[] rangeX= new sbyte[9];
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 9)]
	public sbyte[] rangeY= new sbyte[9];
	[MarshalAs(UnmanagedType.U1)]
	public sbyte effectNum;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 9)]
	public sbyte[] effectX= new sbyte[9];
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 9)]
	public sbyte[] effectY= new sbyte[9];
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class ActHero : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte heroIdx;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte skillIdx;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte x;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte y;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class EnemySkillShot : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte heroIdx;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte skillIdx;
}
}
