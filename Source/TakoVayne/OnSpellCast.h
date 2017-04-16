#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "VayneOption.h"

PLUGIN_EVENT(void) OnSpellCast(CastedSpell const& Args)
{
	//if (Args.Caster_ == Me)
	//{
	//	if (Contains(Args.Name_, "VayneTumble") && GOrbwalking->GetOrbwalkingMode() != kModeNone)
	//	{
	//		GOrbwalking->ResetAA();
	//	}
	//}
	//else
		if (Args.Caster_->IsHero() && Args.Caster_->IsEnemy(Me) && Args.Target_ == Me)
	{
		if (antiMelee->Enabled() && Q->IsReady() && Args.Caster_->IsValidTarget(Me, 300) && Me->HealthPercent() <= 40)
		{
			if (Args.Caster_->GetHealth() < GDamage->GetAutoAttackDamage(Me, Args.Caster_, true) * 2)
			{
				return;
			}

			if (CanMove(Me))
			{
				Q->CastOnPosition(Me->GetPosition().Extend(Args.Caster_->GetPosition(), -300));
			}
		}
	}
}
