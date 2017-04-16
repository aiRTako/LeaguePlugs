#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "VayneOption.h"

PLUGIN_EVENT(void) OnCreateObj(IUnit* source)
{
	if (source->IsHero() && source->IsEnemy(Me) && !HaveShield(source) && E->IsReady())
	{
		if (antiRengar->Enabled() && Contains(source->GetObjectName(), "Rengar_LeapSound.troy") && Distance(Me, source) < E->Range())
		{
			E->CastOnUnit(source);
		}

		if (antiKhazix->Enabled() && Contains(source->GetObjectName(), "Khazix_Base_E_Tar.troy") && Distance(Me, source) <= 300)
		{
			E->CastOnUnit(source);
		}
	}
}
