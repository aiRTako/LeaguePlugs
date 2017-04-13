#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "RivenOption.h"

inline static void KillStealOnUpdate()
{
	if (killStealW->Enabled() && W->IsReady())
	{
		for (auto target : GEntityList->GetAllHeros(false, true))
		{
			if (target == nullptr || target->IsDead() || !target->IsValidTarget(Me, W->GetSpellRange()) || target->IsInvulnerable() || IsUnKillable(target))
			{
				return;
			}

			auto DMG = GDamage->GetSpellDamage(Me, target, kSlotW);

			if (!target->IsDead() && DMG > target->GetHealth())
			{
				W->CastOnPlayer();
				return;
			}
		}
	}

	if (killStealR->Enabled() && R2->IsReady() && isRActive())
	{
		for (auto target : GEntityList->GetAllHeros(false, true))
		{
			if (target == nullptr || target->IsDead() || !target->IsValidTarget(Me, R2->Range()) || target->IsInvulnerable() || IsUnKillable(target))
			{
				return;
			}

			auto DMG = GDamage->GetSpellDamage(Me, target, kSlotR);

			if (!target->IsDead() && DMG > target->GetHealth() && killStealList[target->GetNetworkId()]->Enabled())
			{
				AdvPredictionOutput pred;
				R2->RunPrediction(target, true, kCollidesWithYasuoWall, &pred);

				if (pred.HitChance >= kHitChanceHigh)
				{
					R2->CastOnPosition(pred.CastPosition);
				}
			}
		}
	}
}