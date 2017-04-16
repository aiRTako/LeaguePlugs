#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "VayneOption.h"

PLUGIN_EVENT(void) OnGapcloser(GapCloserSpell const& Args)
{
	if (!antiGap->Enabled() || !E->IsReady() || Args.Source == nullptr || !Args.Source->IsHero() || !Args.Source->IsEnemy(Me))
	{
		return;
	}

	if (!Args.Source->IsDead() && Args.Source->IsValidTarget())
	{
		if (Args.Source->IsValidTarget(Me, E->Range()))
		{
			if (antiAlista->Enabled() && Contains(Args.Source->ChampionName(), "Alistar") && Args.IsTargeted && Distance(Me, Args.EndPosition) <= 300)
			{
				E->CastOnUnit(Args.Source);
			}

			if (!Contains(Args.Source->ChampionName(), "Khazix") &&
				!Contains(Args.Source->ChampionName(), "Rengar") &&
				!Contains(Args.Source->ChampionName(), "MasterYi") &&
				!Contains(Args.Source->ChampionName(), "Alistar") &&
				antiGapcloserList[Args.Source->GetNetworkId()]->Enabled())
			{
				E->CastOnUnit(Args.Source);
			}
		}
	}
}