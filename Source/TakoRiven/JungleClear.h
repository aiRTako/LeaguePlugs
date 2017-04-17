#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "RivenOption.h"

inline static void JungleClearOnUpdate()
{
	if (JungleClearW->Enabled() && W->IsReady() && qStack > 0)
	{
		if (GetMobsCount(Me, GetWRange()) > 0 && W->CastOnPlayer())
		{
			return;
		}
	}

	if (JungleClearW->Enabled() && W->IsReady() && qStack > 0)
	{
		if (GetMobsCount(Me, (float)((Me->AttackRange() + Me->BoundingRadius())* 2.5)) > 0)
		{
			for (auto mob : GEntityList->GetAllMinions(false, false, true))
			{
				if (mob != nullptr && mob->IsValidTarget(Me, 400.0f))
				{
					if ((!Q->IsReady() && !W->IsReady()) || DistanceToPlayer(mob) > 250)
					{
						E->CastOnPosition(GGame->CursorPosition());
					}
				}
			}
		}
	}
}

inline static void AfterJungleClear(IUnit* tar)
{
	if (tar == nullptr || tar->IsDead())
	{
		return;
	}

	if (GetMobsCount(Me, 400.0f) > 0)
	{
		for (auto mob : GEntityList->GetAllMinions(false, false, true))
		{
			if (mob->IsValidTarget(Me, 600.0f))
			{
				if (JungleClearItem->Enabled())
				{
					UseItem(false);
				}

				if (JungleClearQ->Enabled() && Q->IsReady() && mob->IsValidTarget(Me, 400.0f))
				{
					CastQ(mob);
				}
				else if (JungleClearW->Enabled() && W->IsReady() && mob->IsValidTarget(Me, GetWRange()))
				{
					W->CastOnPlayer();
				}
				else if (JungleClearE->Enabled() && E->IsReady() && mob->IsValidTarget(Me, 400.0f))
				{
					E->CastOnPosition(GGame->CursorPosition());
				}
			}
		}
	}
}