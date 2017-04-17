#pragma once

#include "PluginSDK.h"
#include "RivenOption.h"

PLUGIN_EVENT(void) OnInterrupt(InterruptibleSpell const& Args)
{
	if (!Interr->Enabled() || !W->IsReady() || Args.Source == nullptr || !Args.Source->IsHero() || !Args.Source->IsEnemy(Me))
	{
		return;
	}

	if (Args.Source->IsValidTarget(Me, GetWRange()) && Args.DangerLevel == kHighDanger)
	{
		W->CastOnPlayer();
	}
}