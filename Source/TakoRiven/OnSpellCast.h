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
		//	(GOrbwalking->GetOrbwalkingMode() != kModeNone || GetAsyncKeyState(BurstKey->GetInteger())))
		//{
		//	GOrbwalking->ResetAA();
		//}

		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			ComboSpellCast(Args);
		}

		if (GetAsyncKeyState(BurstKey->GetInteger()))
		{
			BurstSpellCast(Args);
		}
	}
	// just C&P that, bitch
	else if (Args.Caster_->IsHero() && Args.Caster_->IsEnemy(Me) && Args.Target_ == Me)
	{
		if (!EShield->Enabled() || !E->IsReady())
		{
			return;
		}

		for (auto spellName : dodgeSpellNameData)
		{
			if (std::string(Args.Name_) == spellName)
			{
				if (E->CastOnPosition(GGame->CursorPosition()))
				{
					return;
				}
			}
		}
	}
}
