#pragma once

#include "PluginSDK.h"
#include "RivenOption.h"

#include "Combo.h"
#include "Burst.h"

PLUGIN_EVENT(void) OnDoCast(CastedSpell const& Args)
{
	if (Args.Caster_ != Me)
	{
		return;
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
	{
		ComboDoCast(Args);
	}
	
	if (GetAsyncKeyState(BurstKey->GetInteger()))
	{
		BurstDoCast(Args);
	}
}