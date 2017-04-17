#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "RivenOption.h"

inline static void ShyBurstOnUpdate()
{
	auto target = GTargetSelector->GetFocusedTarget();

	if (target != nullptr && !target->IsDead() && target->IsValidTarget(Me, 800) && !target->IsInvulnerable())
	{
		UseYoumuu();

		if (R->IsReady() && !isRActive() && !target->IsDead())
		{
			if (Q->IsReady() && E->IsReady() && W->IsReady() &&
				Distance(target, Me->ServerPosition()) < E->Range() + Me->AttackRange() + 100)
			{
				E->CastOnPosition(target->GetPosition());
			}

			if (E->IsReady() && Distance(target, Me->ServerPosition()) < Me->AttackRange() + E->Range() + 100)
			{
				R->CastOnPlayer();
				E->CastOnPosition(target->GetPosition());
			}
		}

		if (W->IsReady() && !target->IsDead() && target->IsValidTarget(Me, GetWRange()))
		{
			W->CastOnPlayer();
		}

		if ((qStack == 1 || qStack == 2 || target->HealthPercent() < 50) && !target->IsDead() && isRActive())
		{
			R2->CastOnPosition(target->ServerPosition());

			if (Q->IsReady() && !target->IsDead())
			{
				Q->CastOnPosition(target->ServerPosition());
			}
		}

		if (BurstDot->Enabled() && haveDot() && Ignite->IsReady() && !target->IsDead())
		{
			if (target->HealthPercent() < 50 && !target->IsDead())
			{
				Ignite->CastOnUnit(target);
			}
		}

		if (BurstFlash->Enabled() && haveFlash() && Flash->IsReady() && !target->IsDead())
		{
			if (R2->IsReady() && !isRActive() && E->IsReady() && W->IsReady() && 
				Distance(target, Me->ServerPosition()) <= E->Range() + 425 &&
				Distance(target, Me->ServerPosition()) >= E->Range() + Me->AttackRange() + 85)
			{
				R->CastOnPlayer();
				E->CastOnPosition(target->GetPosition());
				GPluginSDK->DelayFunctionCall(150, []() {
					auto flashPos = GTargetSelector->GetFocusedTarget()->GetPosition();
					Flash->CastOnPosition(flashPos);
				});
			}
		}
	}
}

inline static void EQFlashBurstOnUpdate()
{
	auto target = GTargetSelector->GetFocusedTarget();

	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me))
		return;

	if (BurstDot &&  haveDot() && Ignite->IsReady() && target->IsValidTarget(Me, 600.0f))
	{
		Ignite->CastOnUnit(target);
	}

	if (BurstFlash && haveFlash() && Flash->IsReady())
	{
		if (target->IsValidTarget(Me, E->Range() + 425.0f + Q->Range() - 150.0f) &&
			qStack == 2 && E->IsReady() && R->IsReady() && !isRActive() && W->IsReady())
		{
			E->CastOnPosition(target->GetPosition());
			GPluginSDK->DelayFunctionCall(10, []()
			{
				R->CastOnPlayer();
			});
			GPluginSDK->DelayFunctionCall(50, []()
			{
				auto target1 = GTargetSelector->GetFocusedTarget();

				if (target1 == nullptr || target1->IsDead() || !target1->IsHero() || !target1->IsEnemy(Me) || !Flash->IsReady())
					return;

				Flash->CastOnPosition(target1->GetPosition());
			});
			GPluginSDK->DelayFunctionCall(61, []()
			{
				auto target1 = GTargetSelector->GetFocusedTarget();

				if (target1 == nullptr || target1->IsDead() || !target1->IsHero() || !target1->IsEnemy(Me) || !Q->IsReady())
					return;

				Q->CastOnPosition(target1->GetPosition());
			});
			GPluginSDK->DelayFunctionCall(62, []()
			{
				UseItem(true);
			});
			GPluginSDK->DelayFunctionCall(70, []()
			{
				if (W->IsReady())
					W->CastOnPlayer();
			});
			GPluginSDK->DelayFunctionCall(71, []()
			{
				auto target1 = GTargetSelector->GetFocusedTarget();

				if (target1 == nullptr || target1->IsDead() || !target1->IsHero() || !target1->IsEnemy(Me))
					return;

				if (R2->CastOnPosition(target1->GetPosition()))
				{
					if (GOrbwalking->GetOrbwalkingMode() == kModeCombo && Q->IsReady())
					{
						Q->CastOnPosition(target1->GetPosition());
					}
				}
			});
			return;
		}
		else
		{
			if (qStack < 2 && GGame->TickCount() - lastQTime >= 850)
			{
				Q->CastOnPosition(GGame->CursorPosition());
			}

			if (GetEnemiesCount(Me, GetWRange()) > 0 && W->IsReady())
			{
				W->CastOnPlayer();
			}
		}
	}
	else
	{
		if (target->IsValidTarget(Me, E->Radius() + Q->Range() - 150.0f) &&
			qStack == 2 && E->IsReady() && R->IsReady() && !isRActive() && W->IsReady())
		{
			E->CastOnPosition(target->GetPosition());
			GPluginSDK->DelayFunctionCall(10, []()
			{
				R->CastOnPlayer();
			});
			GPluginSDK->DelayFunctionCall(50, []()
			{
				auto target1 = GTargetSelector->GetFocusedTarget();

				if (target1 == nullptr || target1->IsDead() || !target1->IsHero() || !target1->IsEnemy(Me) || !Q->IsReady())
					return;

				Q->CastOnPosition(target1->GetPosition());
			});
			GPluginSDK->DelayFunctionCall(61, []()
			{
				UseItem(true);
			});
			GPluginSDK->DelayFunctionCall(62, []()
			{
				if (W->IsReady())
					W->CastOnPlayer();
			});
			GPluginSDK->DelayFunctionCall(70, []()
			{
				auto target1 = GTargetSelector->GetFocusedTarget();

				if (target1 == nullptr || target1->IsDead() || !target1->IsHero() || !target1->IsEnemy(Me) || !R2->IsReady())
					return;

				if (R2->CastOnPosition(target1->GetPosition()))
				{
					if (GOrbwalking->GetOrbwalkingMode() == kModeCombo && Q->IsReady())
					{
						Q->CastOnPosition(target1->GetPosition());
					}
				}
			});
			return;
		}
		else if (GetEnemiesCount(Me, GetWRange()) > 0 && W->IsReady())
		{
			W->CastOnPlayer();
		}
		else if (target->IsValidTarget(Me, E->Range() + Q->Range() + Q->Range() + Q->Range()))
		{
			if (qStack < 2 && GGame->TickCount() - lastQTime >= 850)
			{
				Q->CastOnPosition(GGame->CursorPosition());
			}
		}
	}
}

inline static void BurstMove()
{
	auto target = GTargetSelector->GetFocusedTarget();

	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me))
	{
		GGame->IssueOrder(Me, kMoveTo, GGame->CursorPosition());
		return;
	}

	if (!target->IsDead() && target->IsValidTarget(Me, Me->GetRealAutoAttackRange(target) + target->BoundingRadius()))
	{
		GOrbwalking->Orbwalk(target, GGame->CursorPosition());
		return;
	}

	GGame->IssueOrder(Me, kMoveTo, GGame->CursorPosition());
}

inline static void BurstOnUpdate()
{
	BurstMove();

	auto target = GTargetSelector->GetFocusedTarget();

	if (target != nullptr && !target->IsDead() && target->IsValidTarget(Me, 1500))
	{
		if (BurstMode->GetInteger() == 0)
		{
			ShyBurstOnUpdate();
		}
		else
		{
			EQFlashBurstOnUpdate();
		}
	}
}

inline static void AfterBurst()
{
	auto target = GTargetSelector->GetFocusedTarget();

	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me))
		return;

	UseItem(true);

	if (Q->IsReady() && !target->IsDead())
	{
		CastQ(target);
	}
	else if (W->IsReady() && !target->IsDead() && target->IsValidTarget(Me, GetWRange()))
	{
		W->CastOnPlayer();
	}
	else if (E->IsReady() && !target->IsDead())
	{
		E->CastOnPosition(GGame->CursorPosition());
	}

	//if (R->IsReady() && isRActive())
	//{
	//	if (R2->CastOnPosition(target->GetPosition()))
	//	{
	//		if (Q->IsReady())
	//		{
	//			Q->CastOnPosition(target->GetPosition());
	//		}
	//	}
	//}

	//CastQ(target);

	//if (W->IsReady() && target->IsValidTarget(Me, GetWRange()))
	//{
	//	W->CastOnPlayer();
	//	return;
	//}

	//if (E->IsReady())
	//{
	//	E->CastOnPosition(target->GetPosition());
	//}
}

inline static void EQFlashDoCast(CastedSpell const& Args)
{
	auto target = GTargetSelector->GetFocusedTarget();

	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me))
		return;

	if (Contains(Args.Name_, "RivenIzunaBlade"))
	{
		if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
		{
			Q->CastOnPosition(target->GetPosition());
		}
	}

	if (Contains(Args.Name_, "RivenMartyr"))
	{
		if (R->IsReady() && isRActive())
		{
			if (R2->CastOnPosition(target->GetPosition()))
			{
				if (Q->IsReady())
				{
					Q->CastOnPosition(target->GetPosition());
				}
			}
		}

		if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
		{
			CastQ(target);
		}
	}

	if (Contains(Args.Name_, "ItemTiamatCleave"))
	{
		if (W->IsReady() && target->IsValidTarget(Me, GetWRange()))
		{
			W->CastOnPlayer();
		}

		if (R->IsReady() && isRActive())
		{
			if (R2->CastOnPosition(target->GetPosition()))
			{
				if (Q->IsReady())
				{
					Q->CastOnPosition(target->GetPosition());
				}
			}
		}

		if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
		{
			CastQ(target);
		}
	}
}

inline static void BurstDoCast(CastedSpell const& Args)
{
	auto target = GTargetSelector->GetFocusedTarget();

	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me))
		return;

	if (target->IsValidTarget(Me, 600))
	{
		if (BurstMode->GetInteger() == 1)
		{
			EQFlashDoCast(Args);
		}
	}
}

inline static void BurstSpellCast(CastedSpell const& Args)
{
	auto target = GTargetSelector->GetFocusedTarget();

	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me))
		return;

	if (target->IsValidTarget(Me, 600.0f))
	{
		if (Contains(Args.Name_, "RivenFeint"))
		{
			if (R->IsReady() && !isRActive())
			{
				R->CastOnPlayer();
			}
		}

		if (Contains(Args.Name_, "ItemTiamatCleave"))
		{
			if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
			{
				CastQ(target);
			}
		}

		//if (Contains(Args.Name_, "RivenMartyr"))
		//{
		//	if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
		//	{
		//		CastQ(target);
		//	}
		//}
	}
}