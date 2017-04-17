#pragma once

#include "PluginSDK.h"

ISpell* Flash;
ISpell* Ignite;

ISpell2* Q;
ISpell* W;
ISpell2* E;
ISpell* R;
ISpell2* R2;

IMenu* MainMenu;

IMenu* ComboMenu;
IMenuOption* ComboQ;
IMenuOption* ComboW;
IMenuOption* ComboWLogic;
IMenuOption* ComboE;
IMenuOption* ComboEGap;
IMenuOption* ComboR1;
IMenuOption* ComboR1Ex;
IMenuOption* ComboR2;
IMenuOption* ComboDot;
IMenuOption* ComboItem;

IMenu* BurstMenu;
IMenuOption* BurstFlash;
IMenuOption* BurstDot;
IMenuOption* BurstMode;
IMenuOption* BurstKey;

IMenu* HarassMenu;
IMenuOption* HarassQ;
IMenuOption* HarassW;
IMenuOption* HarassE;
IMenuOption* HarassMode;

IMenu* LaneClearMenu;
IMenuOption* LaneClearQ;
IMenuOption* LaneClearQSmart;
IMenuOption* LaneClearQTurret;
IMenuOption* LaneClearW;
IMenuOption* LaneClearWCount;
IMenuOption* LaneClearItem;

IMenu* JungleClearMenu;
IMenuOption* JungleClearQ;
IMenuOption* JungleClearW;
IMenuOption* JungleClearE;
IMenuOption* JungleClearItem;

IMenu* FleeMenu;
IMenuOption* FleeQ;
IMenuOption* FleeW;
IMenuOption* FleeE;
IMenuOption* FleeKey;

IMenu* KillStealMenu;
IMenuOption* KillStealW;
IMenuOption* KillStealR;

IMenu* MiscMenu;
IMenuOption* KeepQ;
IMenuOption* QMode;
IMenuOption* AntiGap;
IMenuOption* Interr;
IMenuOption* EShield;
IMenuOption* ManualCancel;
IMenuOption* CalculatePing;

IMenu* DrawMenu;
IMenuOption* DrawW;
IMenuOption* DrawR;
IMenuOption* DrawDMG;

IUnit* Me;

IInventoryItem* Tiamat;
IInventoryItem* Titanic;
IInventoryItem* Ravenous;
IInventoryItem* Youmuu;

static int qStack = 0;
static int lastQTime = 0;

static std::vector<std::string> dodgeSpellNameData =
{
	"dariusexecute", "monkeykingspintowin", "pantheonw", "vaynecondemm",
	"vaynesilvereddebuff", "rocketgrab2","bluecardpreattack", "redcardpreattack",
	"katarinartrigger", "hungeringstrike","goldcardpreattack",
	"jaycethunderingblow", "renektonsuperexecute", "veigarprimordialBurst",
	"twitcheparticle", "rengarpassivebuffdashaadummy", "volibearw",
	"hecarimrampattack", "illaoiwattack", "jaxempowertwo",
	"rengarpassivebuffdash", "ireliaequilibriumstrike", "braumbasicattackpassiveoverride", "gnarwproc",
	"monkeykingqattack", "yasuodash", "fizzpiercingstrike",
	"rengarq", "garenqattack", "garenrprecast", "blindmonkrkick",
	"poppypassiveattack", "viktorqbuff", "fioraeattack",
	"teemoq", "caitlynaceinthehole", "headbutt", "khazixq", "khazixqlong",
	"renektonpreexecute", "taloncutthroat", "fallenone",
	"xenzhaothrust3", "elisespiderqcast", "feast", "olafrecklessstrike"
};

void InitMenu()
{
	MainMenu = GPluginSDK->AddMenu("TakoRiven");

	ComboMenu = MainMenu->AddMenu("Riven:Combo");
	{
		ComboQ = ComboMenu->CheckBox("Combo Q Gapcloser", true);
		ComboW = ComboMenu->CheckBox("Combo W", true);
		ComboWLogic = ComboMenu->CheckBox("Combo W Cancel Animation", true);
		ComboE = ComboMenu->CheckBox("Combo E", true);
		ComboEGap = ComboMenu->CheckBox("Combo E Gapcloser", true);
		ComboR1 = ComboMenu->CheckBox("Combo R1", true);
		ComboR1Ex = ComboMenu->CheckBox("Combo R1 if target need R DMG to KS", true);
		ComboR2 = ComboMenu->AddSelection("Combo R2 Mode", 0, { "Logic", "First Cast", "Only KillSteal", "Off" });
		ComboItem = ComboMenu->CheckBox("Combo Item", true);
		ComboDot = ComboMenu->CheckBox("Combo Ignite", true);
	}

	BurstMenu = MainMenu->AddMenu("Riven:Burst");
	{
		BurstFlash = BurstMenu->CheckBox("Burst Flash", true);
		BurstDot = BurstMenu->CheckBox("Burst Ignite", true);
		BurstMode = BurstMenu->AddSelection("Burst Mode", 0, { "Shy", "EQ Flash" });
		BurstKey = BurstMenu->AddKey("Burst Key", 84);//84 = T Key
	}

	HarassMenu = MainMenu->AddMenu("Riven:Harass");
	{
		HarassQ = HarassMenu->CheckBox("Harass Q", true);
		HarassW = HarassMenu->CheckBox("Harass W", true);
		HarassE = HarassMenu->CheckBox("Harass E", true);
		HarassMode = HarassMenu->AddSelection("Harass Mode", 0, { "Smart EQ return", "Normal" });
	}

	LaneClearMenu = MainMenu->AddMenu("Riven:LaneClear");
	{
		LaneClearQ = LaneClearMenu->CheckBox("LaneClear Q", true);
		LaneClearQSmart = LaneClearMenu->CheckBox("LaneClear Q Smart Farm", true);
		LaneClearQTurret = LaneClearMenu->CheckBox("LaneClear Q Reset Attack Turret", true);
		LaneClearW = LaneClearMenu->CheckBox("LaneClear W", true);
		LaneClearWCount = LaneClearMenu->AddFloat("LaneClear W Min Hit Count >= x", 1, 5, 3);
		LaneClearItem = LaneClearMenu->CheckBox("LaneClear Item", true);
	}

	JungleClearMenu = MainMenu->AddMenu("Riven:JungleClear");
	{
		JungleClearQ = JungleClearMenu->CheckBox("JungleClear Q", true);
		JungleClearW = JungleClearMenu->CheckBox("JungleClear W", true);
		JungleClearE = JungleClearMenu->CheckBox("JungleClear E", true);
		JungleClearItem = JungleClearMenu->CheckBox("JungleClear Item", true);
	}

	FleeMenu = MainMenu->AddMenu("Riven:Flee");
	{
		FleeQ = FleeMenu->CheckBox("Flee Q", true);
		FleeW = FleeMenu->CheckBox("Flee W", true);
		FleeE = FleeMenu->CheckBox("Flee E", true);
		FleeKey = FleeMenu->AddKey("Flee Key", 90); //90 = Z key
	}

	KillStealMenu = MainMenu->AddMenu("Riven:KillSteal");
	{
		KillStealW = KillStealMenu->CheckBox("KillSteal W", true);
		KillStealR = KillStealMenu->CheckBox("KillSteal R2", true);
	}

	MiscMenu = MainMenu->AddMenu("Riven:Misc");
	{
		KeepQ = MiscMenu->CheckBox("Keep Q Alive", true);
		QMode = MiscMenu->AddSelection("Q Mode", 0, { "To target", "To mouse" });
		AntiGap = MiscMenu->CheckBox("Use W Anti Gapcloser", true);
		Interr = MiscMenu->CheckBox("Use W Interrupt Danger Spell", true);
		EShield = MiscMenu->CheckBox("Use E Shield Some Spell", true);
		ManualCancel = MiscMenu->CheckBox("Manual Cancel Animation", true);
		CalculatePing = MiscMenu->CheckBox("Cancel Animation Calculate Ping", true);
	}

	DrawMenu = MainMenu->AddMenu("Riven:Drawings");
	{
		DrawW = DrawMenu->CheckBox("Draw W Range", false);
		DrawR = DrawMenu->CheckBox("Draw R2 Range", false);
		DrawDMG = DrawMenu->CheckBox("Draw Spell Damage", true);
	}
}

void InitSpell()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kCircleCast, false, true, kCollidesWithNothing);
	Q->SetSkillshot(0.25, 100, 2200, 325);

	W = GPluginSDK->CreateSpell(kSlotW, 260);

	E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, false, true, kCollidesWithNothing);
	E->SetOverrideRange(325);

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

inline static float GetWRange()
{
	if (GEntityList->Player()->GetSpellLevel(1) == 0)
	{
		return 0;
	}

	if (GEntityList->Player()->HasBuff("RivenFengShuiEngine"))
	{
		return 330;
	}

	return 260;
}

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

	if (QMode->GetInteger() == 0)
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

	if (!target->IsValidTarget(GEntityList->Player(), 500.0f) || isRActive() || !ComboR1->Enabled())
	{
		return;
	}

	if (ComboR1Ex->Enabled())
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

	switch (ComboR2->GetInteger())
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