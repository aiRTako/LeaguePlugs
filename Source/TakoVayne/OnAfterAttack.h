#pragma once

#include "PluginSDK.h"
#include "VayneOption.h"

#include "Combo.h"
#include "Harass.h"
#include "LaneClear.h"
#include "JungleClear.h"

PLUGIN_EVENT(void) OnAfterAttack(IUnit* unit, IUnit* target)
{
	if (unit != Me)
	{
		return;
	}

	if (target == nullptr || target->IsDead())
	{
		return;
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
	{
		AfterCombo(target);
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
	{
		AfterHarass(target);
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
	{
		AfterLaneClear(target);
		AfterJungleClear(target);
	}
}
