#include "PluginSDK.h"
#include <string>
#include <algorithm>

using namespace std;

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

inline bool IsUnKillable(IUnit* target)
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

inline void WriteChat(std::string const msg)
{
	const char* c_s = msg.c_str();

	GGame->PrintChat(c_s);
}

inline void WriteChat(const char* msg)
{
	GGame->PrintChat(msg);
}

#pragma endregion

#pragma region Distance & DistanceToPlayer

inline float Distance(Vec3 from, Vec3 to)
{
	return (from - to).Length2D();
}

inline float Distance(IUnit* from, IUnit* to)
{
	return (from->GetPosition() - to->GetPosition()).Length2D();
}

inline float Distance(IUnit* from, Vec3 to)
{
	return (from->GetPosition() - to).Length2D();
}

inline float Distance(Vec2 from, Vec2 to)
{
	return (from - to).Length();
}

inline float DistanceToPlayer(IUnit* target)
{
	return (target->GetPosition() - GEntityList->Player()->GetPosition()).Length2D();
}

inline float DistanceToPlayer(Vec3 Position)
{
	return (Position - GEntityList->Player()->GetPosition()).Length2D();
}

#pragma endregion

#pragma region Get Enemies/Allies Count

inline int GetEnemiesCount(IUnit* source, float range)
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

inline int GetEnemiesCount(Vec3 position, float range)
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

inline int GetAlliesCount(IUnit* source, float range)
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

inline int GetAlliesCount(Vec3 position, float range)
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

inline int GetMinionsCount(IUnit* target, float range)
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

inline int GetMinionsCount(Vec3 pos, float range)
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

inline int GetMobsCount(IUnit* target, float range)
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

inline int GetMobsCount(Vec3 pos, float range)
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

inline std::string ToLower(std::string str)
{
	auto result = str;

	for (int i = 0; i < result[i]; i++)
	{
		result[i] = tolower(result[i]);
	}
	
	return result;
}

inline bool Contains(std::string a, std::string b)
{
	auto aLower = ToLower(a);
	auto bLower = ToLower(b);

	return aLower.find(bLower) != std::string::npos;
}

inline bool Equals(std::string a, std::string b)
{
	return strcmp(a.c_str(), b.c_str()) == 0;
}

#pragma endregion