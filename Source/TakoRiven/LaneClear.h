#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "RivenOption.h"

inline static void LaneClearOnUpdate()
{
	if (laneclearQ->Enabled() && laneclearQSmart->Enabled() && Q->IsReady())
	{
		Vec3 pos;
		int count;
		Q->FindBestCastPosition(true, false, pos, count);

		if (count >= 2)
		{
			if (GGame->CurrentTick() - lastQTime > 1200)
			{
				Q->CastOnPosition(pos);
				return;
			}
		}
	}

	if (laneclearW->Enabled() && W->IsReady())
	{
		if (GetMinionsCount(Me, W->GetSpellRange()) >= laneclearWCount->GetFloat())
		{
			W->CastOnPlayer();
		}
	}
}

inline static void AfterLaneClear(IUnit* tar)
{
	if (tar == nullptr || tar->IsDead())
	{
		return;
	}

	if (laneclearQTurret->Enabled() && tar->IsTurret() && GetEnemiesCount(Me, 800.0f) == 0)
	{
		Q->CastOnPosition(Me->GetPosition().Extend(tar->GetPosition(), Q->Range()));
	}
	else
	{
		if (laneclearItem->Enabled() && GetMinionsCount(Me, 400.0f) >= 2)
		{
			UseItem(false);
		}

		if (laneclearQ->Enabled() && Q->IsReady() && GetMinionsCount(Me, 400.0f) >= 2)
		{
			Vec3 pos;
			int count;
			Q->FindBestCastPosition(true, false, pos, count);

			if (count >= 2)
			{
				Q->CastOnPosition(pos);
			}
		}
	}
}