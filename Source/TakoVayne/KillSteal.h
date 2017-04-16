#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "VayneOption.h"

inline static void KillStealOnUpdate()
{
	if (killStealE->Enabled() && E->IsReady())
	{
		for (auto target : GEntityList->GetAllHeros(false, true))
		{
			if (target == nullptr || target->IsDead() || !target->IsValidTarget(Me, E->Range()) || target->IsInvulnerable() || IsUnKillable(target) || HaveShield(target))
			{
				return;
			}

			if (!target->IsDead() && GetRealDMG(target) > (target->GetHealth() + target->HPRegenRate() * 2 + 15))
			{
				E->CastOnUnit(target);
				return;
			}
		}
	}
}