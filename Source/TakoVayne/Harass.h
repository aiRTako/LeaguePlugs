#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "VayneOption.h"

inline static void HarassOnUpdate()
{
	if (Me->ManaPercent() < harassMana->GetFloat() || !CanUseSpell())
	{
		return;
	}

	if (E->IsReady())
	{
		if (harassE2Passive->Enabled())
		{
			auto target = GTargetSelector->FindTarget(MostStacks, PhysicalDamage, E->Range());

			if (target != nullptr && !target->IsDead() && !target->IsInvulnerable() && target->IsValidTarget(Me, E->Range()) && !HaveShield(target))
			{
				if (have2WStack(target))
				{
					E->CastOnUnit(target);
				}
			}
		}

		if (harassEStun->Enabled())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, E->Range());

			if (target != nullptr && !target->IsDead() && !target->IsInvulnerable() && target->IsValidTarget(Me, E->Range()) && !HaveShield(target))
			{
				if (CondemnCheck(target))
				{
					E->CastOnUnit(target);
				}
			}
		}
	}
}

inline static void AfterHarass(IUnit* target)
{
	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me))
		return;

	if (harassQ->Enabled() && Q->IsReady())
	{
		if (harassQ2Passive->Enabled() && !have2WStack(target))
		{
			return;
		}

		QLogic(target, true, true);
	}
}