#pragma once

#include "PluginSDK.h"

#include <map>
#include <string>

ISpell* Flash;
ISpell* Ignite;

ISpell2* Q;
ISpell* W;
ISpell2* E;
ISpell* R;
ISpell2* R2;

IMenu* mainMenu;

IMenu* comboMenu;
IMenuOption* comboQ;
IMenuOption* comboW;
IMenuOption* comboWLogic;
IMenuOption* comboE;
IMenuOption* comboEGap;
IMenuOption* comboR1;
IMenuOption* comboR1Ex;
IMenuOption* comboR2;
IMenuOption* comboDot;
IMenuOption* comboItem;

IMenu* burstMenu;
IMenuOption* burstFlash;
IMenuOption* burstDot;
IMenuOption* burstMode;
IMenuOption* burstKey;

IMenu* harassMenu;
IMenuOption* harassQ;
IMenuOption* harassW;
IMenuOption* harassE;
IMenuOption* harassMode;

IMenu* laneclearMenu;
IMenuOption* laneclearQ;
IMenuOption* laneclearQSmart;
IMenuOption* laneclearQTurret;
IMenuOption* laneclearW;
IMenuOption* laneclearWCount;
IMenuOption* laneclearItem;

IMenu* jungleClearMenu;
IMenuOption* jungleClearQ;
IMenuOption* jungleClearW;
IMenuOption* jungleClearE;
IMenuOption* jungleClearItem;

IMenu* fleeMenu;
IMenuOption* fleeQ;
IMenuOption* fleeW;
IMenuOption* fleeE;
IMenuOption* fleeKey;

IMenu* killStealMenu;
IMenuOption* killStealW;
IMenuOption* killStealR;
std::map<int, IMenuOption*> killStealList;

IMenu* miscMenu;
IMenuOption* keepQ;
IMenuOption* qMode;
IMenuOption* antiGap;
IMenuOption* interr;
IMenuOption* eShield;
IMenuOption* manualCancel;
IMenuOption* calculatePing;

IMenu* drawMenu;
IMenuOption* drawW;
IMenuOption* drawR;
IMenuOption* drawDMG;

IUnit* Me;

IInventoryItem* Tiamat;
IInventoryItem* Titanic;
IInventoryItem* Ravenous;
IInventoryItem* Youmuu;

static int qStack = 0;
static int lastQTime = 0;

static std::vector<std::string> dodgeSpell =
{
	"dariusexecute", "monkeykingspintowin", "pantheonw", "vaynecondemm",
	"vaynesilvereddebuff", "rocketgrab2","bluecardpreattack", "redcardpreattack",
	"katarinartrigger", "hungeringstrike","goldcardpreattack",
	"jaycethunderingblow", "renektonsuperexecute", "veigarprimordialburst",
	"twitcheparticle", "rengarpassivebuffdashaadummy", "volibearw",
	"hecarimrampattack", "illaoiwattack", "jaxempowertwo",
	"rengarpassivebuffdash", "ireliaequilibriumstrike", "braumbasicattackpassiveoverride", "gnarwproc",
	"monkeykingqattack", "yasuodash", "fizzpiercingstrike",
	"rengarq", "garenqattack", "garenrprecast", "blindmonkrkick",
	"poppypassiveattack", "viktorqbuff", "fioraeattack",
	"teemoq", "caitlynaceinthehole", "headbutt", "khazixq", "khazixqlong",
	"renektonpreexecute", "taloncutthroat", "fallenone",
	"xenzhaothrust3", "elisespiderqcast", "feast", "olafrecklessstrike", "TOADDMORE"
};

inline static bool haveFlash()
{
	return  GEntityList->Player()->GetSpellSlot("SummonerFlash") != kSlotUnknown;
}

inline static bool haveDot()
{
	return GEntityList->Player()->GetSpellSlot("SummonerDot") != kSlotUnknown;
}

inline static bool isRActive()
{
	return Contains(GEntityList->Player()->GetSpellBook()->GetName(kSlotR), "RivenIzunaBlade");
}

inline static double GetPassive()
{
	auto level = GEntityList->Player()->GetLevel();

	if (level == 18)
	{
		return 0.5;
	}
	else if (level >= 15)
	{
		return 0.45;
	}
	else if (level >= 12)
	{
		return 0.4;
	}
	else if (level >= 9)
	{
		return 0.35;
	}
	else if (level >= 6)
	{
		return 0.3;
	}
	else if (level >= 3)
	{
		return 0.25;
	}
	else
		return 0.2;
}

inline static float GetComboDMG(IUnit* target, bool includeR = true)
{
	if (target->IsDead() || target->IsInvulnerable())
	{
		return 0;
	}

	double DMG = 0;

	if (Q->IsReady() || GEntityList->Player()->HasBuff("RivenTriCleave"))
	{
		auto qhan = 3 - qStack;

		DMG += (GDamage->GetSpellDamage(GEntityList->Player(), target, kSlotQ) * qhan +
			GDamage->GetAutoAttackDamage(GEntityList->Player(), target, false) * qhan * (1 + GetPassive()));
	}

	if (W->IsReady())
	{
		DMG += GDamage->GetSpellDamage(GEntityList->Player(), target, kSlotW);
	}

	if (E->IsReady())
	{
		DMG += GDamage->GetAutoAttackDamage(GEntityList->Player(), target, true);
	}

	if (R->IsReady() && includeR)
	{
		DMG += GDamage->GetSpellDamage(GEntityList->Player(), target, kSlotR);
	}

	if (haveDot() && Ignite->IsReady())
	{
		DMG += GDamage->GetSummonerSpellDamage(GEntityList->Player(), target, kSummonerSpellIgnite);
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

inline static void UseItem(bool isBurstMode)
{
	if (isBurstMode)
	{
		if (Tiamat->IsOwned() && Tiamat->IsReady())
		{
			Tiamat->CastOnPlayer();
			{
				auto target = GOrbwalking->GetLastTarget();
				if (Q->IsReady() && target != nullptr && !target->IsDead())
				{
					Q->CastOnPosition(target->GetPosition());
				}
			}
		}
		else if (Titanic->IsOwned() && Titanic->IsReady())
		{
			Titanic->CastOnPlayer();
			{
				auto target = GOrbwalking->GetLastTarget();
				if (Q->IsReady() && target != nullptr && !target->IsDead())
				{
					Q->CastOnPosition(target->GetPosition());
				}
			}
		}
		else if (Ravenous->IsOwned() && Ravenous->IsReady())
		{
			Ravenous->CastOnPlayer();
			{
				auto target = GOrbwalking->GetLastTarget();
				if (Q->IsReady() && target != nullptr && !target->IsDead())
				{
					Q->CastOnPosition(target->GetPosition());
				}
			}
		}
	}
	else
	{
		if (Tiamat->IsOwned() && Tiamat->IsReady())
		{
			Tiamat->CastOnPlayer();
		}
		else if (Titanic->IsOwned() && Titanic->IsReady())
		{
			Titanic->CastOnPlayer();
		}
		else if (Ravenous->IsOwned() && Ravenous->IsReady())
		{
			Ravenous->CastOnPlayer();
		}
	}
}

inline static void CastQ(IUnit* target)
{
	if (target == nullptr || target->IsDead() || !Q->IsReady())
	{
		return;
	}

	if (qMode->GetInteger() == 0)
	{
		if (!target->IsDead())
			Q->CastOnPosition(target->GetPosition());

		return;
	}

	if (!target->IsDead())
		Q->CastOnPosition(GGame->CursorPosition());
}

inline static void R1Logic(IUnit* target)
{
	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(GEntityList->Player()))
		return;

	if (!target->IsValidTarget(GEntityList->Player(), 500.0f) || isRActive() || !comboR1->Enabled())
	{
		return;
	}

	if (comboR1Ex->Enabled())
	{
		if (target->GetHealth() <= (GetComboDMG(target, false) + GDamage->GetAutoAttackDamage(GEntityList->Player(), target, true)))
		{
			return;
		}
	}

	if (!target->IsDead())
		R->CastOnPlayer();
}

inline static void R2Logic(IUnit* target)
{
	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(GEntityList->Player()))
		return;

	if (!target->IsValidTarget(GEntityList->Player(), R2->Range()) || !isRActive())
		return;

	switch (comboR2->GetInteger())
	{
	case 0:
		if (target->HealthPercent() < 20 ||
			(target->GetHealth() > GDamage->GetSpellDamage(GEntityList->Player(), target, kSlotR) + GDamage->GetAutoAttackDamage(GEntityList->Player(), target, true) * 2 &&
				target->HealthPercent() < 40) ||
				(target->GetHealth() <= GDamage->GetSpellDamage(GEntityList->Player(), target, kSlotR)) ||
			(target->GetHealth() <= GetComboDMG(target)))
		{
			AdvPredictionOutput pred;
			R2->RunPrediction(target, true, kCollidesWithYasuoWall, &pred);

			if (pred.HitChance >= kHitChanceHigh)
			{
				if (R2->CastOnPosition(pred.CastPosition))
				{
					if (GOrbwalking->GetOrbwalkingMode() == kModeCombo && Q->IsReady())
					{
						Q->CastOnPosition(target->GetPosition());
					}
				}
			}
		}
		break;
	case 1:
		if (DistanceToPlayer(target) < 600)
		{
			AdvPredictionOutput pred;
			R2->RunPrediction(target, true, kCollidesWithYasuoWall, &pred);

			if (pred.HitChance >= kHitChanceHigh)
			{
				if (R2->CastOnPosition(pred.CastPosition))
				{
					if (GOrbwalking->GetOrbwalkingMode() == kModeCombo && Q->IsReady())
					{
						Q->CastOnPosition(target->GetPosition());
					}
				}
			}
		}
		break;
	case 2:
		if (GDamage->GetSpellDamage(GEntityList->Player(), target, kSlotR) > target->GetHealth() && DistanceToPlayer(target) < 600)
		{
			AdvPredictionOutput pred;
			R2->RunPrediction(target, true, kCollidesWithYasuoWall, &pred);

			if (pred.HitChance >= kHitChanceHigh)
			{
				if (R2->CastOnPosition(pred.CastPosition))
				{
					if (GOrbwalking->GetOrbwalkingMode() == kModeCombo && Q->IsReady())
					{
						Q->CastOnPosition(target->GetPosition());
					}
				}
			}
		}
		break;
	}
}

inline static void InitSpells()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kCircleCast, false, true, kCollidesWithNothing);
	Q->SetSkillshot(0.25, 100, 2200, 325);

	W = GPluginSDK->CreateSpell(kSlotW, 260);

	E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, false, true, kCollidesWithNothing);
	E->SetOverrideRange(312);

	R = GPluginSDK->CreateSpell(kSlotR);

	R2 = GPluginSDK->CreateSpell2(kSlotR, kConeCast, false, true, kCollidesWithYasuoWall);
	R2->SetSkillshot(0.25, 45, 1600, 900);

	if (GEntityList->Player()->GetSpellSlot("SummonerFlash") != kSlotUnknown)
	{
		Flash = GPluginSDK->CreateSpell(GEntityList->Player()->GetSpellSlot("SummonerFlash"), 425);
	}

	if (GEntityList->Player()->GetSpellSlot("SummonerDot") != kSlotUnknown)
	{
		Ignite = GPluginSDK->CreateSpell(GEntityList->Player()->GetSpellSlot("SummonerDot"), 600);
	}

	Youmuu = GPluginSDK->CreateItemForId(3142, 0);
	Tiamat = GPluginSDK->CreateItemForId(3077, 400);
	Titanic = GPluginSDK->CreateItemForId(3748, 400);
	Ravenous = GPluginSDK->CreateItemForId(3074, 400);
}

inline static void InitMenu()
{
	mainMenu = GPluginSDK->AddMenu("TakoRiven Plus");

	comboMenu = mainMenu->AddMenu("Combo Settings");
	{
		comboQ = comboMenu->CheckBox("Use Q Gapcloser", true);
		comboW = comboMenu->CheckBox("Use W", true);
		comboWLogic = comboMenu->CheckBox("Use W Cancel Animation", true);
		comboE = comboMenu->CheckBox("Use E", true);
		comboEGap = comboMenu->CheckBox("Use E Gapcloser", true);
		comboR1 = comboMenu->CheckBox("Use R1", true);
		comboR1Ex = comboMenu->CheckBox("Use R1 (if target need R DMG to KS)", true);
		comboR2 = comboMenu->AddSelection("Use R2 Mode: ", 0, { "Logic", "First Cast", "Only KillSteal", "Off" });
		comboItem = comboMenu->CheckBox("Use Item", true);
		comboDot = comboMenu->CheckBox("Use Ignite", true);
	}

	burstMenu = mainMenu->AddMenu("Burst Settings");
	{
		burstFlash = burstMenu->CheckBox("Use Flash", true);
		burstDot = burstMenu->CheckBox("Use Ignite", true);
		burstMode = burstMenu->AddSelection("Burst Mode: ", 0, { "Shy", "EQ Flash" });
		burstKey = burstMenu->AddKey("Burst Key", 84);//84 = T Key
		auto burst1 = burstMenu->CheckBox("You need to left click target", false);
		auto burst2 = burstMenu->CheckBox("then press burst key", false);
	}

	harassMenu = mainMenu->AddMenu("Harass Settings");
	{
		harassQ = harassMenu->CheckBox("Use Q", true);
		harassW = harassMenu->CheckBox("Use W", true);
		harassE = harassMenu->CheckBox("Use E", true);
		harassMode = harassMenu->AddSelection("Harass Mode: ", 0, { "Smart(EQ return)", "Normal" });
	}

	laneclearMenu = mainMenu->AddMenu("LaneClear Settings");
	{
		laneclearQ = laneclearMenu->CheckBox("Use Q", true);
		laneclearQSmart = laneclearMenu->CheckBox("Use Q (Smart Farm)", true);
		laneclearQTurret = laneclearMenu->CheckBox("Use Q Reset Attack Turret", true);
		laneclearW = laneclearMenu->CheckBox("Use W", true);
		laneclearWCount = laneclearMenu->AddFloat("Use W Min Hit Count >= x", 1, 5, 3);
		laneclearItem = laneclearMenu->CheckBox("Use Item", true);
	}

	jungleClearMenu = mainMenu->AddMenu("JungleClear Settings");
	{
		jungleClearQ = jungleClearMenu->CheckBox("Use Q", true);
		jungleClearW = jungleClearMenu->CheckBox("Use W", true);
		jungleClearE = jungleClearMenu->CheckBox("Use E", true);
		jungleClearItem = jungleClearMenu->CheckBox("Use Item", true);
	}

	fleeMenu = mainMenu->AddMenu("Flee Settings");
	{
		fleeQ = fleeMenu->CheckBox("Use Q", true);
		fleeW = fleeMenu->CheckBox("Use W", true);
		fleeE = fleeMenu->CheckBox("Use E", true);
		fleeKey = fleeMenu->AddKey("Flee Key", 90); //90 = Z key
	}

	killStealMenu = mainMenu->AddMenu("KillSteal Settings");
	{
		killStealW = killStealMenu->CheckBox("Use W", true);
		killStealR = killStealMenu->CheckBox("Use R2", true);

		for (auto target : GEntityList->GetAllHeros(false, true))
		{
			if (target != nullptr)
			{
				std::string szMenuName = "R2 KS: " + std::string(target->ChampionName());
				killStealList[target->GetNetworkId()] = killStealMenu->CheckBox(szMenuName.c_str(), true);
			}
		}
	}

	miscMenu = mainMenu->AddMenu("Misc Settings");
	{
		keepQ = miscMenu->CheckBox("Keep Q Alive", true);
		qMode = miscMenu->AddSelection("Q Mode: ", 0, { "To target", "To mouse" });
		antiGap = miscMenu->CheckBox("Use W Anti Gapcloser", true);
		interr = miscMenu->CheckBox("Use W Interrupt Danger Spell", true);
		eShield = miscMenu->CheckBox("Use E Shield Some Spell", true);
		manualCancel = miscMenu->CheckBox("Manual Cancel Animation", true);
		calculatePing = miscMenu->CheckBox("Cancel Animation Calculate Ping", true);
	}

	drawMenu = mainMenu->AddMenu("Drawings Settings");
	{
		drawW = drawMenu->CheckBox("Draw W Range", false);
		drawR = drawMenu->CheckBox("Draw R2 Range", false);
		drawDMG = drawMenu->CheckBox("Draw Spell Damage", true);
	}
}
