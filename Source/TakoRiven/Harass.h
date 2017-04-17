#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "RivenOption.h"

inline static void HarassOnUpdate()
{
	auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, 600.0f);

	if (target == nullptr || target->IsDead() || target->IsInvulnerable())
	{
		return;
	}

	if (HarassMode->GetInteger() == 0)
	{
		if (E->IsReady() && HarassE->Enabled() && qStack == 2)
		{
			auto pos = Me->GetPosition() + (Me->GetPosition() - target->GetPosition()).VectorNormalize() * E->Range();

			E->CastOnPosition(Me->GetPosition().Extend(pos, E->Range()));
		}

		if (Q->IsReady() && HarassQ->Enabled() && qStack == 2)
		{
			GPluginSDK->DelayFunctionCall(100, []()
			{
				auto target1 = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, 600.0f);

				if (target1 != nullptr && !target1->IsDead() && !target1->IsInvulnerable())
				{
					Q->CastOnPosition(
						Me->GetPosition().Extend(
							Me->GetPosition() + (Me->GetPosition() - target1->GetPosition()).VectorNormalize() * E->Range(),
							Q->Range()));
				}
			});
		}

		if (W->IsReady() && HarassW->Enabled() && !target->IsDead() && target->IsValidTarget(Me, GetWRange()) && qStack == 1)
		{
			W->CastOnPlayer();
		}

		if (Q->IsReady() && HarassQ->Enabled() && !target->IsDead())
		{
			if (qStack == 0)
			{
				CastQ(target);
				GOrbwalking->SetOverrideTarget(target);
			}

			if (qStack == 1 && GGame->TickCount() - lastQTime > 600)
			{
				CastQ(target);
				GOrbwalking->SetOverrideTarget(target);
			}
		}
	}
	else
	{
		if (E->IsReady() && HarassE->Enabled() && !target->IsDead() &&
			DistanceToPlayer(target) <= E->Range() + (Q->IsReady() ? Q->Range() : Me->AttackRange()))
		{
			E->CastOnPosition(target->GetPosition());
		}

		if (Q->IsReady() && HarassQ->Enabled() && !target->IsDead() && target->IsValidTarget(Me, Q->Range()) && qStack == 0 &&
			GGame->TickCount() - lastQTime > 500)
		{
			CastQ(target);
			GOrbwalking->SetOverrideTarget(target);
		}

		if (W->IsReady() && HarassW->Enabled() && !target->IsDead() && target->IsValidTarget(Me, GetWRange()) && (!Q->IsReady() || qStack == 1))
		{
			W->CastOnPlayer();
		}
	}
}

inline static void AfterHarass(IUnit* target)
{
	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me))
		return;

	if (HarassQ->Enabled() && Q->IsReady())
	{
		if (HarassMode->GetInteger() == 0)
		{
			if (qStack == 1)
			{
				if (!target->IsDead())
					CastQ(target);
			}
		}
		else
		{
			if (!target->IsDead())
				CastQ(target);
		}
	}
}