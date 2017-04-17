#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "RivenOption.h"

inline static void LaneClearOnUpdate()
{
	if (LaneClearQ->Enabled() && LaneClearQSmart->Enabled() && Q->IsReady())
	{
		Vec3 pos;
		int count;
		Q->FindBestCastPosition(true, false, pos, count);

		if (count >= 2)
		{
			if (GGame->TickCount() - lastQTime > 1200)
			{
				Q->CastOnPosition(pos);
				return;
			}
		}
	}

	if (LaneClearW->Enabled() && W->IsReady())
	{
		if (GetMinionsCount(Me, GetWRange()) >= LaneClearWCount->GetFloat())
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

	if (LaneClearQTurret->Enabled() && tar->IsTurret() && GetEnemiesCount(Me, 800.0f) == 0)
	{
		Q->CastOnPosition(Me->GetPosition().Extend(tar->GetPosition(), Q->Range()));
	}
	else
	{
		if (LaneClearItem->Enabled() && GetMinionsCount(Me, 400.0f) >= 2)
		{
			UseItem(false);
		}

		if (LaneClearQ->Enabled() && Q->IsReady() && GetMinionsCount(Me, 400.0f) >= 2)
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