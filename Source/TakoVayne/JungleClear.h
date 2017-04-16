#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "VayneOption.h"

inline static void JungleClearOnUpdate()
{
	if (Me->ManaPercent() < jungleClearMana->GetFloat() || !CanUseSpell() || !jungleClearE->Enabled() || !E->IsReady())
	{
		return;
	}

	if (GetMobsCount(Me, E->Range()) > 0)
	{
		for (auto mob : GEntityList->GetAllMinions(false, false, true))
		{
			if (mob != nullptr && mob->IsValidTarget(Me, E->Range()))
			{
				if (Contains(mob->GetBaseSkinName(), "mini") ||
					Contains(mob->GetBaseSkinName(), "baron") ||
					Contains(mob->GetBaseSkinName(), "dragon") ||
					Contains(mob->GetBaseSkinName(), "crab") ||
					Contains(mob->GetBaseSkinName(), "herald"))
				{
					continue;
				}

				if (CondemnCheck(mob))
				{
					E->CastOnUnit(mob);
				}
			}
		}
	}
}

inline static void AfterJungleClear(IUnit* tar)
{
	if (tar == nullptr || tar->IsDead() || !jungleClearQ->Enabled() || !Q->IsReady())
	{
		return;
	}

	if (GetMobsCount(Me, 800) > 0)
	{
		for (auto mob : GEntityList->GetAllMinions(false, false, true))
		{
			if (mob != nullptr && mob->IsValidTarget(Me, 800))
			{
				Q->CastOnPosition(GGame->CursorPosition());
			}
		}
	}
}