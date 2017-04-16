#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "VayneOption.h"

inline static void ComboOnUpdate()
{
	if (Me->IsWindingUp() || Me->IsDashing())
	{
		return;
	}

	if (comboItem->Enabled())
	{
		auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, 600);

		if (target != nullptr && !target->IsDead() && target->IsValidTarget(Me, 600) && !HaveShield(target))
		{
			UseYoumuu();
			UseItem(target);
		}
	}

	if (comboR->Enabled() && R->IsReady() && GetEnemiesCount(Me, 650) >= comboRCount->GetFloat() && Me->HealthPercent() <= comboRHp->GetFloat())
	{
		R->CastOnPlayer();
	}

	if (comboE->Enabled() && E->IsReady())
	{
		ELogic();
	}

	if (comboQDash->Enabled() && Q->IsReady())
	{
		auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, 900);

		if (target != nullptr && !target->IsDead() && target->IsValidTarget(Me, 900) && Distance(target, Me) > GetRealAutoAttackRange(Me, target))
		{
			QLogic(target);
		}
	}
}

inline static void AfterCombo(IUnit* target)
{
	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me))
		return;

	if (comboAQA->Enabled() && Q->IsReady())
	{
		QLogic(target, false, true);
	}
}