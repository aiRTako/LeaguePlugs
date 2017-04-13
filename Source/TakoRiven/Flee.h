#pragma once

#include "PluginSDK.h"
#include "Extensions.h"
#include "RivenOption.h"

inline static void FleeOnUpdate()
{
	GGame->IssueOrder(Me, kMoveTo, GGame->CursorPosition());

	if (fleeW->Enabled() && W->IsReady() && GetEnemiesCount(Me, W->GetSpellRange()) > 0 && W->CastOnPlayer())
	{
		return;
	}

	if (fleeE->Enabled() && E->IsReady() &&
		((!Q->IsReady() && qStack == 0) || qStack == 2) &&
		E->CastOnPosition(Me->GetPosition().Extend(GGame->CursorPosition(), E->Range())))
	{
		return;
	}

	if (fleeQ->Enabled() && Q->IsReady() && !Me->IsDashing())
	{
		Q->CastOnPosition(Me->GetPosition().Extend(GGame->CursorPosition(), 350.0f));
	}
}