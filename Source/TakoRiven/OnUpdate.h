#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "RivenOption.h"

#include "Combo.h"
#include "Burst.h"
#include "Harass.h"
#include "LaneClear.h"
#include "JungleClear.h"
#include "Flee.h"
#include "KillSteal.h"

#include <string>

PLUGIN_EVENT(void) OnUpdate()
{
	if (Me->IsDead())
	{
		qStack = 0;
		return;
	}

	if (qStack != 0 && GGame->TickCount() - lastQTime > 3800)
	{
		qStack = 0;
	}

	if (Me->IsRecalling())
	{
		return;
	}

	if (KeepQ->Enabled() && Me->HasBuff("RivenTriCleave"))
	{
		if (GBuffData->GetEndTime(Me->GetBuffDataByName("RivenTriCleave")) - GGame->Time() < 0.3)
		{
			Q->CastOnPosition(GGame->CursorPosition());
			return;
		}
	}

	KillStealOnUpdate();

	if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
	{
		ComboOnUpdate();
	}

	if (GetAsyncKeyState(BurstKey->GetInteger()))
	{
		BurstOnUpdate();
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

	if (GetAsyncKeyState(FleeKey->GetInteger()))
	{
		FleeOnUpdate();
	}
}
