#pragma once

#include "PluginSDK.h"
#include "VayneOption.h"

PLUGIN_EVENT(void) OnRender()
{
	if (Me->IsDead() || GGame->IsChatOpen() || GGame->IsScoreboardOpen() || GGame->IsShopOpen())
	{
		return;
	}

	if (E->IsReady() && drawE->Enabled())
	{
		GRender->DrawOutlinedCircle(Me->GetPosition(), Vec4(255, 255, 0, 255), E->Range());
	}
}