#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "RivenOption.h"
#include "AnimationCancel.h"

PLUGIN_EVENT(void) OnPlayAnimation(IUnit* source, std::string const Args)
{
	if (source != Me)
	{
		return;
	}

	if (Contains(Args, "c29"))
	{
		qStack = 1;
		lastQTime = GGame->TickCount();
		Q1Q2Cancel();
		return;
	}
	else if (Contains(Args, "c39"))
	{
		qStack = 2;
		lastQTime = GGame->TickCount();
		Q1Q2Cancel();
		return;
	}
	else if (Contains(Args, "c49"))
	{
		qStack = 0;
		lastQTime = GGame->TickCount();
		Q3Cancel();
	}

	if (Contains(Args, "b5f"))
	{
		WCancel();
		return;
	}

	if (Contains(Args, "bea"))
	{
		R1Cancel();
		return;
	}

	if (Contains(Args, "bda"))
	{
		R2Cancel();
		return;
	}

	// Spell E
	//if (Contains(Args, "b4f63b8e"))
}
