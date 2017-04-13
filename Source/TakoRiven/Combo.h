#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "RivenOption.h"

inline static void ComboOnUpdate()
{
	auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, 900.0f);

	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me))
		return;

	if (comboDot->Enabled() && haveDot() && Ignite->IsReady() && target->IsValidTarget(Me, 600.0f) &&
		(target->GetHealth() < GDamage->GetSummonerSpellDamage(Me, target, kSummonerSpellIgnite) ||
		(target->GetHealth() <= GetComboDMG(target) && target->IsValidTarget(Me, 400.0f))))
	{
		Ignite->CastOnUnit(target);
	}

	if (comboItem->Enabled() && target->IsValidTarget(Me, 550.0f))
	{
		UseYoumuu();
	}

	if (comboR1->Enabled() && R->IsReady() && !isRActive() && target->GetHealth() <= GetComboDMG(target) * 1.20 && target->IsValidTarget(Me, 600.0f))
	{
		R1Logic(target);
	}

	if (comboR2->GetInteger() != 3 && R->IsReady() && isRActive())
	{
		R2Logic(target);
	}

	if (comboQ->Enabled() && Q->IsReady() && GGame->CurrentTick() - lastQTime >= 1200 &&
		!Me->IsWindingUp() && target->IsValidTarget(Me, 420.0f) &&
		DistanceToPlayer(target) > (Me->AttackRange() + Me->BoundingRadius() + 100.0f + target->BoundingRadius()))
	{
		Vec3 predPos = Vec3(0, 0, 0);
		GPrediction->GetFutureUnitPosition(target, Q->GetDelay(), true, predPos);

		if (predPos != Vec3(0, 0, 0) &&
			(DistanceToPlayer(predPos) <= DistanceToPlayer(target) ||
			(Distance(target, predPos) <= DistanceToPlayer(target))))
		{
			Q->CastOnPosition(target->GetPosition());
			return;
		}
	}

	if (comboEGap->Enabled() && E->IsReady() && target->IsValidTarget(Me, 600.0f) &&
		DistanceToPlayer(target) > (Me->AttackRange() + Me->BoundingRadius() + 80.0f))
	{
		E->CastOnPosition(target->GetPosition());
		return;
	}

	if (comboWLogic->Enabled() && W->IsReady() && target->IsValidTarget(Me, W->GetSpellRange()))
	{
		if (qStack == 0)
		{
			W->CastOnPlayer();
		}

		if (Q->IsReady() && qStack > 1)
		{
			W->CastOnPlayer();
		}

		if (Me->HasBuff("RivenFeint"))
		{
			W->CastOnPlayer();
		}

		if (!target->IsFacing(Me))
		{
			W->CastOnPlayer();
		}
	}
}

inline static void AfterCombo(IUnit* target)
{
	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me))
		return;

	if (comboItem->Enabled())
	{
		UseItem(false);
	}

	if (comboR2->GetInteger() != 3 && R->IsReady() && isRActive() && qStack == 2 && Q->IsReady())
	{
		R2Logic(target);
	}

	if (target->IsValidTarget(Me, 450.0f))
	{
		CastQ(target);
	}
	else if (comboW->Enabled() && W->IsReady() && target->IsValidTarget(Me, W->GetSpellRange()) && !HaveShield(target))
	{
		W->CastOnPlayer();
	}
	else if (comboE->Enabled() && !Q->IsReady() && !W->IsReady() && E->IsReady() && target->IsValidTarget(Me, 400.0f))
	{
		E->CastOnPosition(target->GetPosition());
	}
	else if (comboR1->Enabled() && R->IsReady() && !isRActive())
	{
		R1Logic(target);
	}
}

inline static void ComboDoCast(CastedSpell const& Args)
{
	auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, 400.0f);

	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me))
		return;

	if (target->IsValidTarget(Me, 400.0f))
	{
		if (Contains(Args.Name_, "ItemTiamatCleave"))
		{
			if (comboW->Enabled() && W->IsReady() && target->IsValidTarget(Me, W->GetSpellRange()))
			{
				W->CastOnPlayer();
			}
			else if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
			{
				CastQ(target);
			}
			return;
		}

		if (Contains(Args.Name_, "RivenMartyr"))
		{
			if (comboR1->Enabled() && R->IsReady() && !isRActive())
			{
				R1Logic(target);
			}
			else if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
			{
				CastQ(target);
			}
			return;
		}

		if (Contains(Args.Name_, "RivenFengShuiEngine"))
		{
			if (comboW->Enabled() && W->IsReady() && target->IsValidTarget(Me, W->GetSpellRange()))
			{
				W->CastOnPlayer();
			}
			return;
		}

		if (Contains(Args.Name_, "RivenIzunaBlade") && qStack == 2)
		{
			if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
			{
				CastQ(target);
			}
			return;
		}
	}
}

inline static void ComboSpellCast(CastedSpell const& Args)
{
	auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, 600.0f);

	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me))
		return;

	if (target->IsValidTarget(Me, 600.0f))
	{
		if (Contains(Args.Name_, "ItemTiamatCleave"))
		{
			if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
			{
				CastQ(target);
			}
		}

		if (Contains(Args.Name_, "RivenFeint"))
		{
			if (comboR1->Enabled() && R->IsReady() && !isRActive() && target->IsValidTarget(Me, 500.0f))
			{
				R1Logic(target);
			}
		}
	}
}