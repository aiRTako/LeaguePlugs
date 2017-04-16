#pragma once

#include "PluginSDK.h"
#include "Extensions.h"

#include <map>
#include <string>

using namespace std;

ISpell* Flash;

ISpell2* Q;
ISpell* W;
ISpell2* E;
ISpell* R;

IMenu* mainMenu;

IMenu* comboMenu;
IMenuOption* comboQDash;
IMenuOption* comboAQA;
IMenuOption* comboE;
IMenuOption* comboR;
IMenuOption* comboRCount;
IMenuOption* comboRHp;
IMenuOption* comboItem;

IMenu* harassMenu;
IMenuOption* harassQ;
IMenuOption* harassQ2Passive;
IMenuOption* harassEStun;
IMenuOption* harassE2Passive;
IMenuOption* harassMana;

IMenu* laneclearMenu;
IMenuOption* laneclearQ;
IMenuOption* laneclearQTurret;
IMenuOption* laneclearMana;

IMenu* jungleClearMenu;
IMenuOption* jungleClearQ;
IMenuOption* jungleClearE;
IMenuOption* jungleClearMana;

IMenu* killStealMenu;
IMenuOption* killStealE;

IMenu* miscMenu;
IMenuOption* antiMelee;
IMenuOption* qTurret;
IMenuOption* qCheck;
IMenuOption* antiGap;
IMenuOption* antiKhazix;
IMenuOption* antiRengar;
IMenuOption* antiAlista;
IMenuOption* interr;
IMenuOption* ePushDistance;
IMenuOption* autoR;
IMenuOption* autoRRange;
IMenuOption* autoRCount;

std::map<int, IMenuOption*> antiGapcloserList;

IMenu* drawMenu;
IMenuOption* drawE;

IUnit* Me;

IInventoryItem* Cutlass;
IInventoryItem* Botrk;
IInventoryItem* Youmuu;

inline static bool haveFlash()
{
	return  GEntityList->Player()->GetSpellSlot("SummonerFlash") != kSlotUnknown;
}

inline static bool isRActive()
{
	return GEntityList->Player()->HasBuff("vayneinquisition");
}

inline static bool haveWBuff(IUnit* target)
{
	return target != nullptr && !target->IsDead() && target->IsValidTarget() && target->HasBuff("VayneSilveredDebuff");
}

inline static bool have2WStack(IUnit* target)
{
	return target != nullptr && !target->IsDead() && target->IsValidTarget() && target->HasBuff("VayneSilveredDebuff") && target->GetBuffCount("VayneSilveredDebuff") == 2;
}

inline static float GetWDMG(IUnit* target)
{
	if (target->IsDead() || target->IsInvulnerable() || !target->IsValidTarget() || 
		GEntityList->Player()->GetSpellBook()->GetLevel(kSlotW) == 0 ||
		!target->HasBuff("vaynesilvereddebuff") ||
		target->GetBuffCount("vaynesilvereddebuff") != 2)
	{
		return 0;
	}

	auto wDMG = std::vector<double>({ 0.06, 0.075, 0.09, 0.105, 0.12 }).at(GEntityList->Player()->GetSpellBook()->GetLevel(kSlotW) - 1)* target->GetMaxHealth();

	return wDMG;
}

inline static float GetRealDMG(IUnit* target, bool includeW = true)
{
	if (target->IsDead() || target->IsInvulnerable())
	{
		return 0;
	}

	double DMG = 0;


	if (E->IsReady())
	{
		auto mod = std::vector<double>({ 45, 80, 115, 150, 185 }).at(GEntityList->Player()->GetSpellBook()->GetLevel(kSlotW) - 1) + 0.5*GEntityList->Player()->PhysicalDamageMod();
		auto eDMG = GDamage->CalcMagicDamage(GEntityList->Player(), target, mod);

		if (includeW && target->HasBuff("VayneSilveredDebuff") && target->GetBuffCount("VayneSilveredDebuff") == 2)
		{
			DMG = eDMG + GetWDMG(target);
		}
		else
		{
			DMG = eDMG;
		}
	}

	if (GEntityList->Player()->HasBuff("SummonerExhaust"))
	{
		DMG *= 0.6;
	}

	if (target->IsHero() && target->ChampionName() == "Moredkaiser")
	{
		DMG -= target->GetMana();
	}

	if (target->IsHero() && target->HasBuff("GarenW"))
	{
		DMG *= 0.7;
	}

	if (target->IsHero() && target->HasBuff("ferocioushowl"))
	{
		DMG *= 0.7;
	}

	if (target->IsHero() && (target->HasBuff("BlitzcrankManaBarrierCD") || target->HasBuff("ManaBarrier")))
	{
		DMG -= target->GetMana() / 2;
	}

	return (float)DMG;
}

inline static void UseYoumuu()
{
	if (Youmuu->IsOwned() && Youmuu->IsReady())
	{
		Youmuu->CastOnPlayer();
	}
}

inline static void UseItem(IUnit* target)
{
	if (Cutlass->IsOwned() && Cutlass->IsReady() && Me->IsValidTarget(target, 600))
	{
		Cutlass->CastOnTarget(target);
	}

	if (Botrk->IsOwned() && Botrk->IsReady() && Me->IsValidTarget(target, 600))
	{
		Botrk->CastOnTarget(target);
	}
}

inline static Vec3 GetDashQPos()
{
	auto firstQPos = GEntityList->Player()->ServerPosition().Extend(GGame->CursorPosition(), 300);
	auto allPoint = CirclePoints(18, 300, GEntityList->Player()->ServerPosition());

	for (auto point : allPoint)
	{
		int mousecount = GetEnemiesCount(firstQPos, 300);
		int count = GetEnemiesCount(point, 300);

		if (!HaveEnemiesInRange(point))
		{
			continue;
		}

		if (mousecount == count)
		{
			if (Distance(GGame->CursorPosition(), point) < Distance(GGame->CursorPosition(), firstQPos))
			{
				firstQPos = point;
			}
		}

		if (count < mousecount)
		{
			firstQPos = point;
		}
	}

	for (auto i = 1; i <= 5; i++)
	{
		if (GNavMesh->IsPointWall(GEntityList->Player()->GetPosition().Extend(firstQPos, i * float(20))))
		{
			return Vec3(0, 0, 0);
		}
	}

	if (qTurret->Enabled() && GUtility->IsPositionUnderTurret(firstQPos))
	{
		return Vec3(0, 0, 0);
	}

	if (qCheck->Enabled())
	{
		if (GetEnemiesCount(GEntityList->Player(), 350) < GetEnemiesCount(firstQPos, 550))
		{
			return Vec3(0, 0, 0);
		}

		if (GetEnemiesCount(firstQPos, 550) > 3)
		{
			return Vec3(0, 0, 0);
		}
	}

	if (!HaveEnemiesInRange(firstQPos))
	{
		return Vec3(0, 0, 0);
	}

	return firstQPos;
}

inline static bool CondemnCheck(IUnit* target, bool checkQE = false)
{
	auto PD = ePushDistance->GetFloat();

	if (checkQE)
	{
		for (int i = 1; i < PD; i += (int)target->BoundingRadius())
		{
			auto qPos = GetDashQPos();
			auto VL = target->ServerPosition().Extend(qPos, -i);
			auto CF = GNavMesh->GetCollisionFlagsForPoint(VL);

			if (CF == kWallMesh || CF == kBuildingMesh || CF == kPropMesh)
			{
				return true;
			}
		}
	}
	else
	{
		for (int i = 1; i < PD; i += (int)target->BoundingRadius())
		{
			auto VL = target->ServerPosition().Extend(GEntityList->Player()->GetPosition(), -i);
			auto CF = GNavMesh->GetCollisionFlagsForPoint(VL);

			if (CF == kWallMesh || CF == kBuildingMesh || CF == kPropMesh)
			{
				return true;
			}
		}
	}

	return false;
}

inline static void ELogic()
{
	for (auto target : GEntityList->GetAllHeros(false, true))
	{
		if (target != nullptr && !target->IsDead() && target->IsValidTarget(GEntityList->Player(), E->Range()) && !HaveShield(target))
		{
			if (CondemnCheck(target, false))
			{
				E->CastOnUnit(target);
				return;
			}
		}
	}
}

inline static void QLogic(IUnit* target, bool before = true, bool after = true)
{
	if (target == nullptr || target->IsDead() || !Q->IsReady())
	{
		return;
	}

	if (before)
	{
		if (isRActive() && GetEnemiesCount(GEntityList->Player(), 1200) > 0 && GetEnemiesCount(GEntityList->Player(), 700) >= 2)
		{
			auto qPos = GetDashQPos();

			if (qPos != Vec3(0, 0, 0))
			{
				if (CanMove(GEntityList->Player()))
				{
					Q->CastOnPosition(qPos);
				}
			}
		}

		if (DistanceToPlayer(target) > GetRealAutoAttackRange(GEntityList->Player(), target))
		{
			if (Distance(GGame->CursorPosition(), target->GetPosition()) <= Distance(GEntityList->Player()->GetPosition(), target->GetPosition()))
			{
				auto qPos = GetDashQPos();

				if (qPos != Vec3(0, 0, 0))
				{
					if (CanMove(GEntityList->Player()))
					{
						Q->CastOnPosition(qPos);
					}
				}
			}
		}

		if (E->IsReady() && GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			auto qPos = GetDashQPos();

			if (qPos != Vec3(0, 0, 0))
			{
				if (CondemnCheck(target, true))
				{
					if (CanMove(GEntityList->Player()))
					{
						Q->CastOnPosition(qPos);
					}
				}
			}
		}
	}

	if (after)
	{
		auto qPos = GEntityList->Player()->ServerPosition().Extend(GGame->CursorPosition(), 300);
		auto disTarget = Distance(qPos, target->ServerPosition());

		if (qTurret->Enabled() && GUtility->IsPositionUnderTurret(qPos))
		{
			return;
		}

		if (qCheck->Enabled() && GetEnemiesCount(qPos, 300) >= 3)
		{
			return;
		}

		if (disTarget >= 300 && disTarget <= 600)
		{
			if (CanMove(GEntityList->Player()))
			{
				Q->CastOnPosition(GGame->CursorPosition());
			}
		}
	}
}

inline static void InitSpells()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, false, false, kCollidesWithNothing);
	Q->SetOverrideRange(300);

	W = GPluginSDK->CreateSpell(kSlotW);

	E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, true, false, kCollidesWithYasuoWall);
	E->SetOverrideRange(650);
	E->SetOverrideDelay(0.25);

	R = GPluginSDK->CreateSpell(kSlotR);

	if (GEntityList->Player()->GetSpellSlot("SummonerFlash") != kSlotUnknown)
	{
		Flash = GPluginSDK->CreateSpell(GEntityList->Player()->GetSpellSlot("SummonerFlash"), 425.0f);
	}

	Cutlass = GPluginSDK->CreateItemForId(3144, 600);
	Botrk = GPluginSDK->CreateItemForId(3153, 600);
	Youmuu = GPluginSDK->CreateItemForId(3142, 0);
}

inline static void InitMenu()
{
	mainMenu = GPluginSDK->AddMenu("TakoVayne Plus");

	comboMenu = mainMenu->AddMenu("Combo Settings");
	{
		comboQDash = comboMenu->CheckBox("Use Q Gapcloser", true);
		comboAQA = comboMenu->CheckBox("Use Q Reset Attack", true);
		comboE = comboMenu->CheckBox("Use E", true);
		comboR = comboMenu->CheckBox("Use R", true);
		comboRCount = comboMenu->AddFloat("Use R (count enemies >= x)", 1, 5, 2);
		comboRHp = comboMenu->AddFloat("Use R (player Healthpercent <= x)", 1, 100, 40);
		comboItem = comboMenu->CheckBox("Use Item", true);
	}

	harassMenu = mainMenu->AddMenu("Harass Settings");
	{
		harassQ = harassMenu->CheckBox("Use Q", true);
		harassQ2Passive = harassMenu->CheckBox("Use Q (only 2 stack)", true);
		harassEStun = harassMenu->CheckBox("Use E (if target can stun)", true);
		harassE2Passive = harassMenu->CheckBox("Use E (if target have 2 stack)", true);
		harassMana = harassMenu->AddFloat("Mana Percent >= x", 0, 100, 50);
	}

	laneclearMenu = mainMenu->AddMenu("LaneClear Settings");
	{
		laneclearQ = laneclearMenu->CheckBox("Use Q", true);
		laneclearQTurret = laneclearMenu->CheckBox("Use Q Reset Attack Turret", true);
		laneclearMana = laneclearMenu->AddFloat("Mana Percent >= x", 0, 100, 50);
	}

	jungleClearMenu = mainMenu->AddMenu("JungleClear Settings");
	{
		jungleClearQ = jungleClearMenu->CheckBox("Use Q", true);
		jungleClearE = jungleClearMenu->CheckBox("Use E", true);
		jungleClearMana = jungleClearMenu->AddFloat("Mana Percent >= x", 0, 100, 30);
	}

	killStealMenu = mainMenu->AddMenu("KillSteal Settings");
	{
		killStealE = killStealMenu->CheckBox("Use E", true);
	}

	miscMenu = mainMenu->AddMenu("Misc Settings");
	{
		auto qsettings = miscMenu->CheckBox("    Q Settings    ", false);
		antiMelee = miscMenu->CheckBox("Use Q Anti Melee", true);
		qTurret = miscMenu->CheckBox("Dont Use Q under turret", true);
		qCheck = miscMenu->CheckBox("Check Q Dash Position is Safe", true);

		auto esettings = miscMenu->CheckBox("    E Settings    ", false);
		ePushDistance = miscMenu->AddFloat("E Push Distance", 380, 475, 420);
		interr = miscMenu->CheckBox("Use E Interrupt Danger Spell", true);
		antiKhazix = miscMenu->CheckBox("Use E Anti Khazix", true);
		antiRengar = miscMenu->CheckBox("Use E Anti Rengar", true);
		antiAlista = miscMenu->CheckBox("Use E Anti Alista", true);
		antiGap = miscMenu->CheckBox("Use E Anti Gapcloser", true);
		for (auto target : GEntityList->GetAllHeros(false, true))
		{
			if (target != nullptr && 
				!Contains(target->ChampionName(), "Khazix") && 
				!Contains(target->ChampionName(), "Rengar") && 
				!Contains(target->ChampionName(), "MasterYi") &&
				!Contains(target->ChampionName(), "Alistar"))
			{
				std::string szMenuName = "Anti Gapcloser: " + std::string(target->ChampionName());
				antiGapcloserList[target->GetNetworkId()] = miscMenu->CheckBox(szMenuName.c_str(), true);
			}
		}

		auto rsettings = miscMenu->CheckBox("    R Settings    ", false);
		autoR = miscMenu->CheckBox("Auto Use R", true);
		autoRCount = miscMenu->AddFloat("Auto Use R(Count Enemies >= x)", 3, 1, 5);
		autoRRange = miscMenu->AddFloat("Auto Use R(Check Range <= x)", 500, 1000, 600);
	}

	drawMenu = mainMenu->AddMenu("Drawings Settings");
	{
		drawE = drawMenu->CheckBox("Draw E Range", false);
	}
}
