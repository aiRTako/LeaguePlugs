#pragma once

#include "PluginSDK.h"
#include "RivenOption.h"

#include "Combo.h"
#include "Burst.h"
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

	if (GetAsyncKeyState(burstKey->GetInteger()))
	{
		AfterBurst();
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
