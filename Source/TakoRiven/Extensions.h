#pragma once

#include "PluginSDK.h"
#include <string>

#pragma region Translate

/*
1. string转const char*
string s = "abc";
const char* c_s = s.c_str();

2. const char*转string
直接赋值即可
const char* c_s = "abc";
string s(c_s);

3. string转char*
string s = "abc";
char* c;
const int len = s.length();
c = new char[len+1];
strcpy(c,s.c_str());

4. char*转string
char* c = "abc";
string s(c);

5. const char*转char*
const char* cpc = "abc";
char* pc = new char[100];//足够长
strcpy(pc,cpc);

6. char*转const char*
直接赋值即可
char* pc = "abc";
const char* cpc = pc;

*/

#pragma endregion

#pragma region Status

inline static bool HaveShield(IUnit* target)
{
	if (target == nullptr || target->IsDead())
	{
		return true;
	}

	if (target->IsInvulnerable())
	{
		return true;
	}

	if (target->HasBuff("SivirE")) // Sivir E
	{
		return true;
	}

	if (target->HasBuff("BlackShield")) // Morgana E
	{
		return true;
	}

	if (target->HasBuff("NocturneShit")) // Noc E
	{
		return true;
	}

	if (target->HasBuff("itemmagekillerveil"))
	{
		return true;
	}

	return false;
}

inline static bool CanMove(IUnit* target)
{
	if (target->MovementSpeed() < 50 || 
		target->HasBuffOfType(BUFF_Stun) || 
		target->HasBuffOfType(BUFF_Fear) || 
		target->HasBuffOfType(BUFF_Snare) || 
		target->HasBuffOfType(BUFF_Knockup) || 
		target->HasBuff("Recall") ||
		target->HasBuffOfType(BUFF_Knockback) || 
		target->HasBuffOfType(BUFF_Charm) || 
		target->HasBuffOfType(BUFF_Taunt) || 
		target->HasBuffOfType(BUFF_Suppression))
	{
		return false;
	}

	return true;
}

inline static bool IsUnKillable(IUnit* target)
{
	if (target == nullptr)
	{
		return true;
	}

	if (target->HasBuff("KindredRNoDeathBuff"))
	{
		return true;
	}

	if (target->HasBuff("UndyingRage"))
	{
		return true;
	}

	if (target->HasBuff("JudicatorIntervention"))
	{
		return true;
	}

	if (target->HasBuff("ChronoShift"))
	{
		return true;
	}

	if (target->HasBuff("ShroudofDarkness"))
	{
		return true;
	}

	if (target->HasBuff("SivirShield"))
	{
		return true;
	}

	if (target->HasBuff("itemmagekillerveil"))
	{
		return true;
	}

	return target->HasBuff("FioraW");
}

#pragma endregion

#pragma region PrintChat

inline static void WriteChat(std::string const msg)
{
	const char* c_s = msg.c_str();

	GGame->PrintChat(c_s);
}

inline static void WriteChat(const char* msg)
{
	GGame->PrintChat(msg);
}

#pragma endregion

#pragma region Distance & DistanceToPlayer

inline static float Distance(Vec3 from, Vec3 to)
{
	float x1 = from.x;
	float x2 = to.x;

	float y1 = from.y;
	float y2 = to.y;

	float z1 = from.z;
	float z2 = to.z;

	return static_cast<float>(sqrt(pow((x2 - x1), 2.0) + pow((y2 - y1), 2.0) + pow((z2 - z1), 2.0)));
}

inline static float Distance(IUnit* from, IUnit* to)
{
	float x1 = from->GetPosition().x;
	float x2 = to->GetPosition().x;

	float y1 = from->GetPosition().y;
	float y2 = to->GetPosition().y;

	float z1 = from->GetPosition().z;
	float z2 = to->GetPosition().z;

	return static_cast<float>(sqrt(pow((x2 - x1), 2.0) + pow((y2 - y1), 2.0) + pow((z2 - z1), 2.0)));
}

inline static float Distance(IUnit* from, Vec3 to)
{
	float x1 = from->GetPosition().x;
	float x2 = to.x;

	float y1 = from->GetPosition().y;
	float y2 = to.y;

	float z1 = from->GetPosition().z;
	float z2 = to.z;

	return static_cast<float>(sqrt(pow((x2 - x1), 2.0) + pow((y2 - y1), 2.0) + pow((z2 - z1), 2.0)));
}

inline static float DistanceToPlayer(IUnit* target)
{
	float x1 = GEntityList->Player()->GetPosition().x;
	float x2 = target->GetPosition().x;

	float y1 = GEntityList->Player()->GetPosition().y;
	float y2 = target->GetPosition().y;

	float z1 = GEntityList->Player()->GetPosition().z;
	float z2 = target->GetPosition().z;

	return static_cast<float>(sqrt(pow((x2 - x1), 2.0) + pow((y2 - y1), 2.0) + pow((z2 - z1), 2.0)));
}

inline static float DistanceToPlayer(Vec3 to)
{
	float x1 = GEntityList->Player()->GetPosition().x;
	float x2 = to.x;

	float y1 = GEntityList->Player()->GetPosition().y;
	float y2 = to.y;

	float z1 = GEntityList->Player()->GetPosition().z;
	float z2 = to.z;

	return static_cast<float>(sqrt(pow((x2 - x1), 2.0) + pow((y2 - y1), 2.0) + pow((z2 - z1), 2.0)));
}

#pragma endregion

#pragma region Get Enemies/Allies Count

inline static int GetEnemiesCount(IUnit* source, float range)
{
	auto Count = 0;

	for (auto target : GEntityList->GetAllHeros(false, true))
	{
		if (target != nullptr && !target->IsDead())
		{
			if (Distance(source, target) <= range)
			{
				Count++;
			}
		}
	}

	return Count;
}

inline static int GetEnemiesCount(Vec3 position, float range)
{
	auto Count = 0;

	for (auto target : GEntityList->GetAllHeros(false, true))
	{
		if (target != nullptr && !target->IsDead())
		{
			if (Distance(target, position) <= range)
			{
				Count++;
			}
		}
	}

	return Count;
}

inline static int GetAlliesCount(IUnit* source, float range)
{
	auto Count = 0;

	for (auto target : GEntityList->GetAllHeros(true, false))
	{
		if (target != nullptr && !target->IsDead())
		{
			if (Distance(source, target) <= range)
			{
				Count++;
			}
		}
	}

	return Count;
}

inline static int GetAlliesCount(Vec3 position, float range)
{
	auto Count = 0;

	for (auto target : GEntityList->GetAllHeros(true, false))
	{
		if (target != nullptr && !target->IsDead())
		{
			if (Distance(target, position) <= range)
			{
				Count++;
			}
		}
	}

	return Count;
}

inline static int GetMinionsCount(IUnit* target, float range)
{
	auto minions = 0;

	for (auto minion : GEntityList->GetAllMinions(false, true, false))
	{
		if (minion != nullptr && minion != target && Distance(target, minion) <= range)
		{
			if (!minion->IsDead())
			{
				minions++;
			}
		}
	}

	return minions;
}

inline static int GetMinionsCount(Vec3 pos, float range)
{
	auto minions = 0;

	for (auto minion : GEntityList->GetAllMinions(false, true, false))
	{
		if (minion != nullptr && Distance(minion, pos) <= range)
		{
			if (!minion->IsDead())
			{
				minions++;
			}
		}
	}

	return minions;
}

inline static int GetMobsCount(IUnit* target, float range)
{
	auto minions = 0;

	for (auto minion : GEntityList->GetAllMinions(false, false, true))
	{
		if (minion != nullptr && minion != target && Distance(target, minion) <= range)
		{
			if (!minion->IsDead())
			{
				minions++;
			}
		}
	}

	return minions;
}

inline static int GetMobsCount(Vec3 pos, float range)
{
	auto minions = 0;

	for (auto minion : GEntityList->GetAllMinions(false, false, true))
	{
		if (minion != nullptr && Distance(minion, pos) <= range)
		{
			if (!minion->IsDead())
			{
				minions++;
			}
		}
	}

	return minions;
}

#pragma endregion

#pragma region string

inline static std::string ToLower(std::string str)
{
	auto result = str;

	for (int i = 0; i < result[i]; i++)
	{
		result[i] = tolower(result[i]);
	}
	
	return result;
}

inline static bool Contains(std::string a, std::string b)
{
	auto aLower = ToLower(a);
	auto bLower = ToLower(b);

	return aLower.find(bLower) != std::string::npos;
}

inline static bool Equals(std::string a, std::string b)
{
	return strcmp(a.c_str(), b.c_str()) == 0;
}

#pragma endregion