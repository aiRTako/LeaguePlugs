#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "RivenOption.h"

inline static void FleeOnUpdate()
{
	GGame->IssueOrder(Me, kMoveTo, GGame->CursorPosition());

	if (FleeW->Enabled() && W->IsReady() && GetEnemiesCount(Me, GetWRange()) > 0 && W->CastOnPlayer())
	{
		return;
	}

	if (FleeE->Enabled() && E->IsReady() &&
		((!Q->IsReady() && qStack == 0) || qStack == 2) &&
		E->CastOnPosition(Me->GetPosition().Extend(GGame->CursorPosition(), E->Range())))
	{
		return;
	}

	if (FleeQ->Enabled() && Q->IsReady() && !Me->IsDashing())
	{
		Q->CastOnPosition(Me->GetPosition().Extend(GGame->CursorPosition(), 350.0f));
	}
}