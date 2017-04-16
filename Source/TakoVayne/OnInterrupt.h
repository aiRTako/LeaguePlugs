#pragma once

#include "PluginSDK.h"
#include "VayneOption.h"

PLUGIN_EVENT(void) OnInterrupt(InterruptibleSpell const& Args)
{
	if (!interr->Enabled() || !E->IsReady() || Args.Source == nullptr || !Args.Source->IsHero() || !Args.Source->IsEnemy(Me))
	{
		return;
	}

	if (Args.Source->IsValidTarget(Me, E->Range()) && Args.DangerLevel == kHighDanger)
	{
		E->CastOnUnit(Args.Source);
	}
}