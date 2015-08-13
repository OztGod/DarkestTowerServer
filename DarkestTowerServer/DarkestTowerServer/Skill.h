#pragma once

struct SkillInfo
{
	int act;
	int cool;
};

struct Skill
{
	int id;
	int level;
	SkillInfo* info;
};