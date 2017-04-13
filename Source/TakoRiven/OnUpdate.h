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

PLUGIN_EVENT(void) OnUpdate()
{
	if (GEntityList->Player()->GetSpellBook()->GetLevel(kSlotW) > 0)
	{
		W->SetSpellRange(Me->HasBuff("RivenFengShuiEngine") ? 330.0f : 260.0f);
	}

	if (qStack != 0 && GGame->CurrentTick() - lastQTime > 3800)
	{
		qStack = 0;
	}

	if (Me->IsDead())
	{
		qStack = 0;
		return;
	}

	if (Me->IsRecalling())
	{
		return;
	}

	if (keepQ->Enabled() && Me->HasBuff("RivenTriCleave"))
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

	if (GetAsyncKeyState(burstKey->GetInteger()))
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

	if (GetAsyncKeyState(fleeKey->GetInteger()))
	{
		FleeOnUpdate();
	}
}
