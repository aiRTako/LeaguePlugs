#pragma once

#include "PluginSDK.h"
#include "RivenOption.h"

PLUGIN_EVENT(void) OnInterrupt(InterruptibleSpell const& Args)
{
	if (!interr->Enabled() || !W->IsReady() || Args.Source == nullptr || !Args.Source->IsHero() || !Args.Source->IsEnemy(Me))
	{
		return;
	}

	if (Args.Source->IsValidTarget(Me, W->GetSpellRange()) && Args.DangerLevel == kHighDanger)
	{
		W->CastOnPlayer();
	}
}