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
	MOVE_HERO = 7,
	CHANGE_HERO_STATE = 8,
	TURN_END = 9,
	UPDATE_TURN = 10,
	TYPE_NUM = 11,
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
	[MarshalAs(UnmanagedType.U1)]
	public char foo;
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
	public sbyte[] skillIdx= new sbyte[4];
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
}
