#pragma once

#include "PluginSDK.h"
#include "RivenOption.h"

PLUGIN_EVENT(void) OnRender()
{
	if (Me->IsDead() || GGame->IsChatOpen() || GGame->IsScoreboardOpen() || GGame->IsShopOpen())
	{
		return;
	}

	if (W->IsReady() && DrawW->Enabled())
	{
		GRender->DrawOutlinedCircle(Me->GetPosition(), Vec4(255, 255, 0, 255), GetWRange());
	}

	if (R2->IsReady() && DrawR->Enabled())
	{
		GRender->DrawOutlinedCircle(Me->GetPosition(), Vec4(255, 255, 0, 255), R2->Range());
	}

	if (DrawDMG->Enabled()) // Credit: Kornis
	{
		for (auto hero : GEntityList->GetAllHeros(false, true))
		{
			Vec2 barPos = Vec2();

			if (!hero->IsDead() && hero->GetHPBarPosition(barPos) && hero->IsOnScreen() && hero->IsValidTarget())
			{
				float percentHealthAfterDamage = max(0, hero->GetHealth() - GetComboDMG(hero)) / hero->GetMaxHealth();
				float yPos = barPos.y + 20;
				float xPosDamage = (barPos.x + 10) + 103 * percentHealthAfterDamage;
				float xPosCurrentHp = barPos.x + 10 + 103 * (hero->GetHealth() / hero->GetMaxHealth());

				if (GetComboDMG(hero) > hero->GetHealth())
				{
					GRender->DrawTextW(Vec2(barPos.x + 10, barPos.y + 15 - 13), Vec4(105, 198, 5, 255), "Killable");
				}

				float differenceInHP = xPosCurrentHp - xPosDamage;
				float pos1 = barPos.x + 9 + (107 * percentHealthAfterDamage);

				for (int i = 0; i < differenceInHP; i++)
				{
					GRender->DrawLine(Vec2(pos1 + i, yPos), Vec2(pos1 + i, yPos + 8), Vec4(198, 176, 5, 255));
				}
			}
		}
	}
}