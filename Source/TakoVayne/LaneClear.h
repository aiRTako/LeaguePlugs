#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "VayneOption.h"

inline static void LaneClearOnUpdate()
{
	if (Me->ManaPercent() < laneclearMana->GetFloat() || !CanUseSpell() || !laneclearQ->Enabled() || !Q->IsReady())
	{
		return;
	}

	if (GetQFarmCount(Me) >= 2)
	{
		Q->CastOnPosition(GGame->CursorPosition());
	}
}

inline static void AfterLaneClear(IUnit* tar)
{
	if (tar == nullptr || tar->IsDead())
	{
		return;
	}

	if (laneclearQTurret->Enabled() && tar->IsTurret() && GetEnemiesCount(Me, 800) == 0)
	{
		Q->CastOnPosition(GGame->CursorPosition());
	}
	else
	{
		if (laneclearQ->Enabled() && Q->IsReady() && GetQFarmCount(Me) >= 2)
		{
			Q->CastOnPosition(GGame->CursorPosition());
		}
	}
}