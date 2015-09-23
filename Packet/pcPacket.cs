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
}
public enum StateType
{
	STATE_IRON = 0,
	STATE_POISON = 1,
	STATE_ICE = 2,
	STATE_BURN = 3,
	STATE_BUFF = 4,
	STATE_TAUNT = 5,
	STATE_SACRIFICE = 6,
	STATE_PRAY = 7,
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
	public sbyte[] x= new sbyte[4];
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
	public sbyte[] y= new sbyte[4];
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class SkillData : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte turn;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte heroIdx;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte skillNum;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
	public byte[] skillType= new byte[4];
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
	public sbyte[] skillLevel= new sbyte[4];
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
	public sbyte maxHp;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte hp;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte maxAct;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte act;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte x;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte y;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte isMove;
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
	public sbyte heroIdx;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte skillIdx;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte isMyField;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte rangeNum;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 9)]
	public sbyte[] rangeX= new sbyte[9];
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 9)]
	public sbyte[] rangeY= new sbyte[9];
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class EffectResponse : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte heroIdx;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte skillIdx;
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
public class SkillShot : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte turn;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte heroIdx;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte skillIdx;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte effectNum;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
	public sbyte[] effectTurn= new sbyte[8];
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
	public sbyte[] effectX= new sbyte[8];
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
	public sbyte[] effectY= new sbyte[8];
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class MatchEnd : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte winner;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class Reject : Header
{
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class DeadHero : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte turn;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte heroIdx;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class HeroState : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public byte stateType;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte targetTurn;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte targetIdx;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte executerTurn;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte executerIdx;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte stateId;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte damaged;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte hp;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte act;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte attack;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte defence;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte duration;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class RemoveHeroState : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte targetTurn;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte targetIdx;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte stateId;
	[MarshalAs(UnmanagedType.U1)]
	public byte stateType;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class RegisterAccountRequest : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte idLength;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
	public char[] id= new char[16];
	[MarshalAs(UnmanagedType.U1)]
	public sbyte passwordLength;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
	public char[] password= new char[16];
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class RegisterAccountResponse : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte isSuccess;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class RequestMatch : Header
{
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class CancelMatch : Header
{
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class EnterLobby : Header
{
	[MarshalAs(UnmanagedType.U4)]
	public Int32 win;
	[MarshalAs(UnmanagedType.U4)]
	public Int32 lose;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte heroNum;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class HeroData : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public byte heroType;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte level;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte hp;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte act;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte skillNum;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
	public sbyte[] skillType= new sbyte[4];
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
	public sbyte[] skillLevel= new sbyte[4];
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class PickTurn : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte pickNum;
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class Pick : Header
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
	public sbyte[] heroIdx= new sbyte[2];
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class PickData : Header
{
	[MarshalAs(UnmanagedType.U1)]
	public sbyte turn;
	[MarshalAs(UnmanagedType.U1)]
	public sbyte heroNum;
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
	public byte[] heroType= new byte[2];
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class PickEnd : Header
{
	[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
	public sbyte[] heroIdx= new sbyte[4];
}
[StructLayout(LayoutKind.Sequential, Pack = 1)]
public class Surrender : Header
{
}
}
