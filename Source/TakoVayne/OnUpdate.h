#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "VayneOption.h"

#include "Combo.h"
#include "Harass.h"
#include "LaneClear.h"
#include "JungleClear.h"
#include "KillSteal.h"

PLUGIN_EVENT(void) OnUpdate()
{
	if (Me->IsDead() || Me->IsRecalling())
	{
		return;
	}

	if (autoR->Enabled() && R->IsReady() && GetEnemiesCount(Me, autoRRange->GetFloat()) >= autoRCount->GetFloat())
	{
		R->CastOnPlayer();
	}

	KillStealOnUpdate();

	if (Me->IsWindingUp())
	{
		return;
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
	{
		ComboOnUpdate();
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
	{
		HarassOnUpdate();
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
	{
		LaneClearOnUpdate();
		JungleClearOnUpdate();
	}
}
