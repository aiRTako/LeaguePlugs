#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "RivenOption.h"

PLUGIN_EVENT(void) OnGapcloser(GapCloserSpell const& Args)
{
	if (!AntiGap->Enabled() || !W->IsReady() || Args.Source == nullptr || !Args.Source->IsHero() || !Args.Source->IsEnemy(Me))
	{
		return;
	}

	if (!Args.Source->IsDead() && !Contains(Args.Source->ChampionName(), "MasterYi") && Args.Source->IsValidTarget(Me, GetWRange()))
	{
		W->CastOnPlayer();
	}
}