#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "RivenOption.h"

#include "Combo.h"
#include "Burst.h"

PLUGIN_EVENT(void) OnSpellCast(CastedSpell const& Args)
{
	if (Args.Caster_ == Me)
	{
		//if ((Contains(Args.Name_, "RivenTriCleave") || Contains(Args.Name_, "ItemTiamatCleave")) &&
		//	(GOrbwalking->GetOrbwalkingMode() != kModeNone || GetAsyncKeyState(burstKey->GetInteger())))
		//{
		//	GOrbwalking->ResetAA();
		//}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			ComboSpellCast(Args);
		}

		if (GetAsyncKeyState(burstKey->GetInteger()))
		{
			BurstSpellCast(Args);
		}
	}
	// just C&P that, bitch
	else if (Args.Caster_->IsHero() && Args.Caster_->IsEnemy(Me) && Args.Target_ == Me)
	{
		if (!eShield->Enabled() || !E->IsReady())
		{
			return;
		}

		if (find(dodgeSpell.begin(), dodgeSpell.end(), Args.Name_) != dodgeSpell.end())
		{
			E->CastOnPosition(GGame->CursorPosition());
		}
	}
}
