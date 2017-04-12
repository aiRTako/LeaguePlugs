#include "PluginSDK.h"
#include "Extensions.h"
#include "stdio.h"
#include "stdlib.h"

PluginSetup("Tako Riven Plus");

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

IMenu* miscMenu;
IMenuOption* keepQ;
IMenuOption* qMode;
IMenuOption* antiGap;
IMenuOption* interr;
IMenuOption* eShield;
IMenuOption* manualCancel;
IMenuOption* calculatePing;

IMenu* drawMenu;

IUnit* Me;

IInventoryItem* Tiamat;
IInventoryItem* Titanic;
IInventoryItem* Ravenous;
IInventoryItem* Youmuu;

int qStack = 0;
int lastQTime = 0;

std::vector<std::string> dodgeSpell = 
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

void InitSpells()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kCircleCast, false, true, kCollidesWithNothing);
	Q->SetSkillshot(0.25f, 100.0f, 2200.0f, 325.0f);

	W = GPluginSDK->CreateSpell(kSlotW, 260.0f);

	E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, false, true, kCollidesWithNothing);
	E->SetOverrideRange(312);

	R = GPluginSDK->CreateSpell(kSlotR);

	R2 = GPluginSDK->CreateSpell2(kSlotR, kConeCast, false, true, kCollidesWithYasuoWall);
	R2->SetSkillshot(0.25, 45, 1600, 900);

	if (GEntityList->Player()->GetSpellSlot("SummonerFlash") != kSlotUnknown)
	{
		Flash = GPluginSDK->CreateSpell(Me->GetSpellSlot("SummonerFlash"), 425.0f);
	}

	if (GEntityList->Player()->GetSpellSlot("SummonerDot") != kSlotUnknown)
	{
		Ignite = GPluginSDK->CreateSpell(Me->GetSpellSlot("SummonerDot"), 600.0f);
	}

	Youmuu = GPluginSDK->CreateItemForId(3142, 0.0f);
	Tiamat = GPluginSDK->CreateItemForId(3077, 400.0f);
	Titanic = GPluginSDK->CreateItemForId(3748, 400.0f);
	Ravenous = GPluginSDK->CreateItemForId(3074, 400.0f);
}

void InitMenu()
{
	mainMenu = GPluginSDK->AddMenu("Tako Riven Plus");

	comboMenu = mainMenu->AddMenu("Combo Settings");
	{
		comboQ = comboMenu->CheckBox("Use Q Gapcloser", true);
		comboW = comboMenu->CheckBox("Use W", true);
		comboWLogic = comboMenu->CheckBox("Use W Cancel Animation", true);
		comboE = comboMenu->CheckBox("Use E", true);
		comboEGap = comboMenu->CheckBox("Use E Gapcloser", true);
		comboR1 = comboMenu->CheckBox("Use R1", true);
		comboR1Ex = comboMenu->CheckBox("Use R1-> If Target Cant normal Kill", true);
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

	}
}

void Q1Q2Cancel()
{
	if (GetAsyncKeyState(fleeKey->GetInteger()))
	{
		return;
	}

	if (manualCancel->Enabled() || GOrbwalking->GetOrbwalkingMode() != kModeNone || GetAsyncKeyState(burstKey->GetInteger()))
	{
		int time = 291;
		int ping = GGame->Latency();

		if (calculatePing->Enabled())
		{
			if (time - ping > 0)
			{
				GPluginSDK->DelayFunctionCall(time - ping, []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
				
					if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() && GOrbwalking->GetLastTarget()->IsEnemy(Me) && GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
					{
						GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
					}
					else
					{
						GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
					}
				});
			}
			else
			{
				GPluginSDK->DelayFunctionCall(1, []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
				
					if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() && GOrbwalking->GetLastTarget()->IsEnemy(Me) && GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
					{
						GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
					}
					else
					{
						GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
					}
				});
			}
		}
		else
		{
			GPluginSDK->DelayFunctionCall(time, []()
			{
				GOrbwalking->ResetAA();
				GGame->Taunt(kDance);
			
				if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() && GOrbwalking->GetLastTarget()->IsEnemy(Me) && GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
				{
					GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
				}
				else
				{
					GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
				}
			});
		}
	}
}

void Q3Cancel()
{
	if (GetAsyncKeyState(fleeKey->GetInteger()))
	{
		return;
	}

	if (manualCancel->Enabled() || GOrbwalking->GetOrbwalkingMode() != kModeNone || GetAsyncKeyState(burstKey->GetInteger()))
	{
		int time = 391;
		int ping = GGame->Latency();

		if (calculatePing->Enabled())
		{
			if (time - ping > 0)
			{
				GPluginSDK->DelayFunctionCall(time - ping, []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
				
					if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() && GOrbwalking->GetLastTarget()->IsEnemy(Me) && GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
					{
						GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
					}
					else
					{
						GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
					}
				});
			}
			else
			{
				GPluginSDK->DelayFunctionCall(1, []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
	
					if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() && GOrbwalking->GetLastTarget()->IsEnemy(Me) && GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
					{
						GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
					}
					else
					{
						GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
					}
				});
			}
		}
		else
		{
			GPluginSDK->DelayFunctionCall(time, []()
			{
				GOrbwalking->ResetAA();
				GGame->Taunt(kDance);
			
				if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() && GOrbwalking->GetLastTarget()->IsEnemy(Me) && GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
				{
					GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
				}
				else
				{
					GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
				}
			});
		}
	}
}

void WCancel()
{
	if (GetAsyncKeyState(fleeKey->GetInteger()))
	{
		return;
	}

	 if (manualCancel->Enabled() || GOrbwalking->GetOrbwalkingMode() != kModeNone || GetAsyncKeyState(burstKey->GetInteger()))
	{
		int time = 50;
		int ping = GGame->Latency();

		if (calculatePing->Enabled())
		{
			if (time - ping > 0)
			{
				GPluginSDK->DelayFunctionCall(time - ping, []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
				});
			}
			else
			{
				GPluginSDK->DelayFunctionCall(1, []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
				});
			}
		}
		else
		{
			GPluginSDK->DelayFunctionCall(time, []()
			{
				GOrbwalking->ResetAA();
				GGame->Taunt(kDance);
			});
		}
	}
}

void R1Cancel()
{
	if (GetAsyncKeyState(fleeKey->GetInteger()))
	{
		return;
	}

	 if (manualCancel->Enabled() || GOrbwalking->GetOrbwalkingMode() != kModeNone || GetAsyncKeyState(burstKey->GetInteger()))
	{
		int time = 50;
		int ping = GGame->Latency();

		if (calculatePing->Enabled())
		{
			if (time - ping > 0)
			{
				GPluginSDK->DelayFunctionCall(time - ping, []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
				
					if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() && GOrbwalking->GetLastTarget()->IsEnemy(Me) && GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
					{
						GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
					}
					else
					{
						GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
					}
				});
			}
			else
			{
				GPluginSDK->DelayFunctionCall(1, []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
				
					if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() && GOrbwalking->GetLastTarget()->IsEnemy(Me) && GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
					{
						GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
					}
					else
					{
						GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
					}
				});
			}
		}
		else
		{
			GPluginSDK->DelayFunctionCall(time, []()
			{
				GOrbwalking->ResetAA();
				GGame->Taunt(kDance);
			
				if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() && GOrbwalking->GetLastTarget()->IsEnemy(Me) && GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
				{
					GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
				}
				else
				{
					GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
				}
			});
		}
	}
}

void R2Cancel()
{
	if (GetAsyncKeyState(fleeKey->GetInteger()))
	{
		return;
	}

	 if (manualCancel->Enabled() || GOrbwalking->GetOrbwalkingMode() != kModeNone || GetAsyncKeyState(burstKey->GetInteger()))
	{
		int time = 180;
		int ping = GGame->Latency();

		if (calculatePing->Enabled())
		{
			if (time - ping > 0)
			{
				GPluginSDK->DelayFunctionCall(time - ping, []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
		
					if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() && GOrbwalking->GetLastTarget()->IsEnemy(Me) && GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
					{
						GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
					}
					else
					{
						GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
					}
				});
			}
			else
			{
				GPluginSDK->DelayFunctionCall(1, []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
				
					if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() && GOrbwalking->GetLastTarget()->IsEnemy(Me) && GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
					{
						GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
					}
					else
					{
						GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
					}
				});
			}
		}
		else
		{
			GPluginSDK->DelayFunctionCall(time, []()
			{
				GOrbwalking->ResetAA();
				GGame->Taunt(kDance);

				if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() && GOrbwalking->GetLastTarget()->IsEnemy(Me) && GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
				{
					GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
				}
				else
				{
					GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
				}
			});
		}
	}
}

bool haveFlash()
{
	return  GEntityList->Player()->GetSpellSlot("SummonerFlash") != kSlotUnknown;
}

bool haveDot()
{
	return GEntityList->Player()->GetSpellSlot("SummonerDot") != kSlotUnknown;
}

bool isRActive()
{
	return Contains(GEntityList->Player()->GetSpellBook()->GetName(kSlotR), "RivenIzunaBlade");
}

double GetPassive()
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

float GetComboDMG(IUnit* target, bool includeR = true)
{
	if (target->IsDead() || target->IsInvulnerable())
	{
		return 0;
	}

	double DMG = 0;

	if (Q->IsReady() || Me->HasBuff("RivenTriCleave"))
	{
		auto qhan = 3 - qStack;

		DMG += (GDamage->GetSpellDamage(Me, target, kSlotQ) * qhan +
			GDamage->GetAutoAttackDamage(GEntityList->Player(), target, false) * qhan * (1 + GetPassive()));
	}

	if (W->IsReady())
	{
		DMG += GDamage->GetSpellDamage(Me, target, kSlotW);
	}

	if (E->IsReady())
	{
		DMG += GDamage->GetAutoAttackDamage(GEntityList->Player(), target, true);
	}

	if (R->IsReady() && includeR)
	{
		DMG += GDamage->GetSpellDamage(Me, target, kSlotR);
	}

	if (haveDot() && Ignite->IsReady())
	{
		DMG += GDamage->GetSummonerSpellDamage(Me, target, kSummonerSpellIgnite);
	}

	return (float)DMG;
}

bool HaveShield(IUnit* target)
{
	if (target == nullptr || target->IsDead())
	{
		return true;
	}

	if (target->IsInvulnerable())
	{
		return true;
	}

	if (target->HasBuff("SivirE")) // Sivir E
	{
		return true;
	}

	if (target->HasBuff("BlackShield")) // Morgana E
	{
		return true;
	}

	if (target->HasBuff("NocturneShit")) // Noc E
	{
		return true;
	}

	if (target->HasBuff("itemmagekillerveil"))
	{
		return true;
	}

	return false;
}

void UseYoumuu()
{
	if (Youmuu->IsOwned() && Youmuu->IsReady())
	{
		Youmuu->CastOnPlayer();
	}
}

void UseItem()
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

void CastQ(IUnit* target)
{
	if (target == nullptr || target->IsDead() || !Q->IsReady())
	{
		return;
	}

	if (qMode->GetInteger() == 0)
	{
		Q->CastOnPosition(target->GetPosition());
	}
	else
		Q->CastOnPosition(GGame->CursorPosition());
}

void R1Logic(IUnit* target)
{
	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me) )
		return;

	if (!target->IsValidTarget(GEntityList->Player(), 500.0f) || isRActive() || !comboR1->Enabled())
	{
		return;
	}

	if (comboR1Ex->Enabled())
	{
		if (target->GetHealth() <= (GetComboDMG(target, false) + GDamage->GetAutoAttackDamage(Me, target, true)))
		{
			return;
		}
	}
	
	R->CastOnPlayer();
}

void R2Logic(IUnit* target)
{
	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me) )
		return;

	if (!target->IsValidTarget(GEntityList->Player(), R2->Range()) || !isRActive())
		return;

	switch (comboR2->GetInteger())
	{
	case 0:
		if (target->HealthPercent() < 20 ||
			(target->GetHealth() > GDamage->GetSpellDamage(Me, target, kSlotR) + GDamage->GetAutoAttackDamage(GEntityList->Player(), target, true) * 2 &&
				target->HealthPercent() < 40) ||
				(target->GetHealth() <= GDamage->GetSpellDamage(Me, target, kSlotR)) ||
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
		if (GDamage->GetSpellDamage(Me, target, kSlotR) > target->GetHealth() && DistanceToPlayer(target) < 600)
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

void Combo()
{
	auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, 900.0f);

	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me) )
		return;

	if (comboDot->Enabled() && 
		haveDot() &&
		Ignite->IsReady() &&
		target->IsValidTarget(Me, 600.0f) &&
		(target->GetHealth() < GDamage->GetSummonerSpellDamage(Me, target, kSummonerSpellIgnite) || 
		(target->GetHealth() <= GetComboDMG(target) && 
			target->IsValidTarget(Me, 400.0f))))
	{
		Ignite->CastOnUnit(target);
	}

	if (comboItem->Enabled() && target->IsValidTarget(Me, 550.0f))
	{
		UseYoumuu();
	}
	
	if (comboR1->Enabled() && 
		R->IsReady() &&
		!isRActive() &&
		target->GetHealth() <= GetComboDMG(target) * 1.20 && 
		target->IsValidTarget(Me, 600.0f))
	{
		R1Logic(target);
	}

	if (comboR2->GetInteger() != 3 && 
		R->IsReady() &&
		isRActive())
	{
		R2Logic(target);
	}

	if (comboQ->Enabled() && 
		Q->IsReady() && 
		GGame->CurrentTick() - lastQTime >= 1200 &&
		!Me->IsWindingUp() && 
		target->IsValidTarget(Me, 420.0f) &&
		DistanceToPlayer(target) > (Me->AttackRange() + Me->BoundingRadius() + 100.0f + target->BoundingRadius()))
	{
		Vec3 predPos = Vec3(0, 0, 0);
		GPrediction->GetFutureUnitPosition(target, Q->GetDelay(), true, predPos);

		if (predPos != Vec3(0, 0, 0) && 
			(DistanceToPlayer(predPos) <= DistanceToPlayer(target) || 
			(Distance(target, predPos) <= DistanceToPlayer(target))))
		{
			Q->CastOnPosition(target->GetPosition());
			return;
		}
	}

	if (comboEGap->Enabled() && 
		E->IsReady() &&
		target->IsValidTarget(Me, 600.0f) && 
		DistanceToPlayer(target) > (Me->AttackRange() + Me->BoundingRadius() + 80.0f))
	{
		E->CastOnPosition(target->GetPosition());
		return;
	}

	if (comboWLogic->Enabled() && 
		W->IsReady() && 
		target->IsValidTarget(Me, W->GetSpellRange()))
	{
		if (qStack == 0)
		{
			W->CastOnPlayer();
		}

		if (Q->IsReady() && qStack > 1)
		{
			W->CastOnPlayer();
		}

		if (Me->HasBuff("RivenFeint"))
		{
			W->CastOnPlayer();
		}

		if (!target->IsFacing(Me))
		{
			W->CastOnPlayer();
		}
	}
}

void ShyBurst()
{
	auto target = GTargetSelector->GetFocusedTarget();

	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me) )
		return;

	if (burstDot &&  haveDot() && Ignite->IsReady() && target->IsValidTarget(Me, 600.0f))
	{
		Ignite->CastOnUnit(target);
	}

	if (E->IsReady() && R->IsReady() &&  W->IsReady() &&  !isRActive())
	{
		if (target->IsValidTarget(Me, E->Range() + Me->BoundingRadius() - 30.0f))
		{
			E->CastOnPosition(target->GetPosition());
			GPluginSDK->DelayFunctionCall(10, []()
			{
				R->CastOnPlayer();
			});
			GPluginSDK->DelayFunctionCall(60, []()
			{
				W->CastOnPlayer();
			});
			GPluginSDK->DelayFunctionCall(70, []()
			{
				auto target1 = GTargetSelector->GetFocusedTarget();
				Q->CastOnPosition(target1->GetPosition());
			});
			GPluginSDK->DelayFunctionCall(80, []()
			{
				auto target1 = GTargetSelector->GetFocusedTarget();
				R2->CastOnPosition(target1->GetPosition());
			});
			return;
		}

		if (burstFlash && haveFlash() && Flash->IsReady())
		{
			if (target->IsValidTarget(Me, E->Range() + Me->BoundingRadius() + 425.0f - 50.0f))
			{
				E->CastOnPosition(target->GetPosition());
				GPluginSDK->DelayFunctionCall(10, []()
				{
					R->CastOnPlayer();
				});
				GPluginSDK->DelayFunctionCall(60, []()
				{
					W->CastOnPlayer();
				});
				GPluginSDK->DelayFunctionCall(61, []()
				{
					auto target1 = GTargetSelector->GetFocusedTarget();
					Flash->CastOnPosition(target1->GetPosition());
				});
				GPluginSDK->DelayFunctionCall(70, []()
				{
					auto target1 = GTargetSelector->GetFocusedTarget();
					Q->CastOnPosition(target1->GetPosition());
				});
				GPluginSDK->DelayFunctionCall(80, []()
				{
					auto target1 = GTargetSelector->GetFocusedTarget();
					R2->CastOnPosition(target1->GetPosition());
				});
				return;
			}
		}
	}
	else
	{
		if (W->IsReady() && target->IsValidTarget(Me, W->GetSpellRange()))
		{
			W->CastOnPlayer();
		}
	}
}

void EQFlashBurst()
{
	auto target = GTargetSelector->GetFocusedTarget();

	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me) )
		return;

	if (burstDot &&  haveDot() && Ignite->IsReady() && target->IsValidTarget(Me, 600.0f))
	{
		Ignite->CastOnUnit(target);
	}
	if (burstFlash &&
		haveFlash() &&
		Flash->IsReady())
	{
		if (target->IsValidTarget(Me, E->Range() + 425.0f + Q->Range() - 150.0f) &&
			qStack == 2 && E->IsReady() &&  R->IsReady() && !isRActive() && W->IsReady())
		{
			E->CastOnPosition(target->GetPosition());
			GPluginSDK->DelayFunctionCall(10, []()
			{
				R->CastOnPlayer();
			});
			GPluginSDK->DelayFunctionCall(50, []()
			{
				auto target1 = GTargetSelector->GetFocusedTarget();
				Flash->CastOnPosition(target1->GetPosition());
			});
			GPluginSDK->DelayFunctionCall(61, []()
			{
				auto target1 = GTargetSelector->GetFocusedTarget();
				Q->CastOnPosition(target1->GetPosition());
			});
			GPluginSDK->DelayFunctionCall(62, []()
			{
				UseItem();
			});
			GPluginSDK->DelayFunctionCall(70, []()
			{
				W->CastOnPlayer();
			});
			GPluginSDK->DelayFunctionCall(71, []()
			{
				auto target1 = GTargetSelector->GetFocusedTarget();
				if (R2->CastOnPosition(target1->GetPosition()))
				{
					if (GOrbwalking->GetOrbwalkingMode() == kModeCombo && Q->IsReady())
					{
						Q->CastOnPosition(target1->GetPosition());
					}
				}
			});
			return;
		}
		else
		{
			if (qStack < 2 && GGame->CurrentTick() - lastQTime >= 850)
			{
				Q->CastOnPosition(GGame->CursorPosition());
			}
		}
	}
	else
	{
		if (target->IsValidTarget(Me, E->Radius() + Q->Range() - 150.0f) &&
			qStack == 2 && E->IsReady() && R->IsReady() &&  !isRActive() && W->IsReady())
		{
			E->CastOnPosition(target->GetPosition());
			GPluginSDK->DelayFunctionCall(10, []()
			{
				R->CastOnPlayer();
			});
			GPluginSDK->DelayFunctionCall(50, []()
			{		
				auto target1 = GTargetSelector->GetFocusedTarget();
				Q->CastOnPosition(target1->GetPosition());
			});
			GPluginSDK->DelayFunctionCall(61, []()
			{
				UseItem();
			});
			GPluginSDK->DelayFunctionCall(62, []()
			{
				W->CastOnPlayer();
			});
			GPluginSDK->DelayFunctionCall(70, []()
			{
				auto target1 = GTargetSelector->GetFocusedTarget();
				if (R2->CastOnPosition(target1->GetPosition()))
				{
					if (GOrbwalking->GetOrbwalkingMode() == kModeCombo && Q->IsReady())
					{
						Q->CastOnPosition(target1->GetPosition());
					}
				}
			});
			return;
		}
		else if (target->IsValidTarget(Me, E->Range() + Q->Range() + Q->Range() + Q->Range()))
		{
			if (qStack < 2 && GGame->CurrentTick() - lastQTime >= 850)
			{
				Q->CastOnPosition(GGame->CursorPosition());
			}
		}
	}
}

void Burst()
{
	auto target = GTargetSelector->GetFocusedTarget();

	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me) )
	{
		GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
		return;
	}

	if (target->IsValidTarget(Me, Me->GetRealAutoAttackRange(target) + target->BoundingRadius()))
	{
		GOrbwalking->Orbwalk(target, GGame->CursorPosition());
	}
	else
	{
		GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
	}

	if (target->IsValidTarget(Me, 1200.0f))
	{
		if (burstMode->GetInteger() == 0)
		{
			ShyBurst();
		}
		else
		{
			EQFlashBurst();
		}
	}
}

void Harass()
{
	IUnit* target = nullptr;

	if (GTargetSelector->GetFocusedTarget() != nullptr && GTargetSelector->GetFocusedTarget()->IsValidTarget(Me, E->Range() + Me->BoundingRadius()))
	{
		target = GTargetSelector->GetFocusedTarget();
	}
	else
	{
		target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, E->Range() + Me->BoundingRadius());
	}

	if (target == nullptr || target->IsDead() || target->IsInvulnerable())
	{
		return;
	}

	if (harassMode->GetInteger() == 0)
	{
		if (E->IsReady() && harassE->Enabled() && qStack == 2)
		{
			Vec3 pos = Me->GetPosition() + (Me->GetPosition() - target->GetPosition()).VectorNormalize() * E->Range();

			E->CastOnPosition(Me->GetPosition().Extend(pos, E->Range()));
		}

		if (Q->IsReady() && harassQ->Enabled() && qStack == 2)
		{
			if (GTargetSelector->GetFocusedTarget() != nullptr)
			{
				GPluginSDK->DelayFunctionCall(100, []()
				{
					Q->CastOnPosition(Me->GetPosition().Extend(Me->GetPosition() + (Me->GetPosition() - GTargetSelector->GetFocusedTarget()->GetPosition()).VectorNormalize() * E->Range(), Q->Range()));
				});
			}
			else
			{
				GPluginSDK->DelayFunctionCall(100, []()
				{
					auto t = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, E->Range() + Me->BoundingRadius());
					Q->CastOnPosition(Me->GetPosition().Extend(Me->GetPosition() + (Me->GetPosition() - t->GetPosition()).VectorNormalize() * E->Range(), Q->Range()));
				});
			}
		}

		if (W->IsReady() && harassW->Enabled() && target->IsValidTarget(Me, W->GetSpellRange()) && qStack == 1)
		{
			W->CastOnPlayer();
		}

		if (Q->IsReady() && harassQ->Enabled())
		{
			if (qStack == 0)
			{
				CastQ(target);
				GOrbwalking->SetOverrideTarget(target);
			}

			if (qStack == 1 && GGame->CurrentTick() - lastQTime > 600)
			{
				CastQ(target);
				GOrbwalking->SetOverrideTarget(target);
			}
		}
	}
	else
	{
		if (E->IsReady() && harassE->Enabled() &&
			DistanceToPlayer(target) <= E->Range() + (Q->IsReady() ? Q->Range() : Me->AttackRange()))
		{
			E->CastOnPosition(target->GetPosition());
		}

		if (Q->IsReady() && harassQ->Enabled() && target->IsValidTarget(Me, Q->Range()) && qStack == 0 &&
			GGame->CurrentTick() - lastQTime > 500)
		{
			CastQ(target);
			GOrbwalking->SetOverrideTarget(target);
		}

		if (W->IsReady() && harassW->Enabled() && target->IsValidTarget(Me, W->GetSpellRange()) && (!Q->IsReady() || qStack == 1))
		{
			W->CastOnPlayer();
		}
	}

}

void LaneClear()
{
	if (laneclearQ->Enabled() && laneclearQSmart->Enabled() && Q->IsReady())
	{
		Vec3 pos;
		int count;
		Q->FindBestCastPosition(true, false, pos, count);

		if (count >= 2)
		{
			if (GGame->CurrentTick() - lastQTime > 1200)
			{
				Q->CastOnPosition(pos);
				return;
			}
		}
	}

	if (laneclearW->Enabled() && W->IsReady())
	{		
		if (GetMinionsCount(Me, W->GetSpellRange()) >= laneclearWCount->GetFloat())
		{
			W->CastOnPlayer();
		}
	}
}

void JungleClear()
{
	if (jungleClearW->Enabled() && W->IsReady() && qStack > 0)
	{
		if (GetMobsCount(Me, W->GetSpellRange()) > 0 && W->CastOnPlayer())
		{
			return;
		}
	}

	if (jungleClearW->Enabled() && W->IsReady() && qStack > 0)
	{
		if (GetMobsCount(Me, (float)((Me->AttackRange() + Me->BoundingRadius())* 2.5)) > 0)
		{
			for (auto mob : GEntityList->GetAllMinions(false, false, true))
			{
				if (mob != nullptr && mob->IsValidTarget(Me, 400.0f))
				{
					if ((!Q->IsReady() && !W->IsReady()) || DistanceToPlayer(mob) > 250)
					{
						E->CastOnPosition(GGame->CursorPosition());
					}
				}
			}
		}
	}
}

void Flee()
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

void AfterCombo(IUnit* target)
{
	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me) )
		return;

	if (comboItem->Enabled())
	{
		UseItem();
	}

	if (comboR2->GetInteger() != 3 && R->IsReady() && isRActive() && qStack == 2 && Q->IsReady())
	{
		R2Logic(target);
	}
	
	if (target->IsValidTarget(Me, 450.0f))
	{
		CastQ(target);
	}
	else if (comboW->Enabled() && W->IsReady() && target->IsValidTarget(Me, W->GetSpellRange()) && !HaveShield(target))
	{
		W->CastOnPlayer();
	}
	else if (comboE->Enabled() && !Q->IsReady() && !W->IsReady() && E->IsReady() && target->IsValidTarget(Me, 400.0f))
	{
		E->CastOnPosition(target->GetPosition());
	}
	else if (comboR1->Enabled() && R->IsReady() && !isRActive())
	{
		R1Logic(target);
	}
}

void AfterBurst()
{
	IUnit* target = GTargetSelector->GetFocusedTarget();

	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me) )
		return;

	UseItem();

	if (R->IsReady() && isRActive())
	{
		if (R2->CastOnPosition(target->GetPosition()))
		{
			if (Q->IsReady())
			{
				Q->CastOnPosition(target->GetPosition());
			}
		}
	}

	CastQ(target);

	if (W->IsReady() && target->IsValidTarget(Me, W->GetSpellRange()))
	{
		W->CastOnPlayer();
		return;
	}

	if (E->IsReady())
	{
		E->CastOnPosition(target->GetPosition());
	}
}

void AfterHarass(IUnit* target)
{
	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me) )
		return;

	if (harassQ->Enabled() && Q->IsReady())
	{
		if (harassMode->GetInteger() == 0)
		{
			if (qStack == 1)
			{
				CastQ(target);
			}
		}
		else
		{
			CastQ(target);
		}
	}
}

void AfterLaneClear(IUnit* tar)
{
	if (tar == nullptr || tar->IsDead())
	{
		return;
	}

	if (laneclearQTurret->Enabled() && tar->IsTurret() && GetEnemiesCount(Me, 800.0f) == 0)
	{
		Q->CastOnPosition(Me->GetPosition().Extend(tar->GetPosition(), Q->Range()));
	}
	else
	{
		if (laneclearItem->Enabled() && GetMinionsCount(Me, 400.0f) >= 2)
		{
			UseItem();
		}

		if (laneclearQ->Enabled() && Q->IsReady() && GetMinionsCount(Me, 400.0f) >= 2)
		{
			Vec3 pos;
			int count;
			Q->FindBestCastPosition(true, false, pos, count);

			if (count >= 2)
			{
				Q->CastOnPosition(pos);
			}
		}
	}
}

void AfterJungleClear(IUnit* tar)
{
	if (tar == nullptr || tar->IsDead())
	{
		return;
	}

	if (GetMobsCount(Me, 400.0f) > 0)
	{
		for (auto mob : GEntityList->GetAllMinions(false, false, true))
		{
			if (mob->IsValidTarget(Me, 600.0f))
			{
				if (jungleClearItem->Enabled())
				{
					UseItem();
				}

				if (jungleClearQ->Enabled() && Q->IsReady() && mob->IsValidTarget(Me, 400.0f))
				{
					CastQ(mob);
				}
				else if (jungleClearW->Enabled() && W->IsReady() && mob->IsValidTarget(Me, W->GetSpellRange()))
				{
					W->CastOnPlayer();
				}
				else if (jungleClearE->Enabled() && E->IsReady() && mob->IsValidTarget(Me, 400.0f))
				{
					E->CastOnPosition(GGame->CursorPosition());
				}
			}
		}
	}
}

void ComboCast(CastedSpell const& Args)
{
	auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, 400.0f);

	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me) )
		return;

	if (target->IsValidTarget(Me, 400.0f))
	{
		if (Contains(Args.Name_, "ItemTiamatCleave"))
		{
			if (comboW->Enabled() && W->IsReady() && target->IsValidTarget(Me, W->GetSpellRange()))
			{
				W->CastOnPlayer();
			}
			else if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
			{
				CastQ(target);
			}
		}
	
		if (Contains(Args.Name_, "RivenMartyr"))
		{
			if (comboR1->Enabled() && R->IsReady() && !isRActive())
			{
				R1Logic(target);
			}
			else if (Q->IsReady() && target->IsValidTarget(Me,400.0f))
			{
				CastQ(target);
			}
		}
	
		if (Contains(Args.Name_, "RivenFengShuiEngine"))
		{
			if (comboW->Enabled() && W->IsReady() && target->IsValidTarget(Me, W->GetSpellRange()))
			{
				W->CastOnPlayer();
			}
		}
	
		if (Contains(Args.Name_, "RivenIzunaBlade") && qStack == 2)
		{
			if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
			{
				CastQ(target);
			}
		}
	}
}

void BurstCast(CastedSpell const& Args)
{
	auto target = GTargetSelector->GetFocusedTarget();

	if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me) )
		return;

	if (target->IsValidTarget(Me, 600.0f))
	{
		if (burstMode->GetInteger() == 0)
		{
			if (Contains(Args.Name_, "ItemTiamatCleave"))
			{
				if (W->IsReady() && target->IsValidTarget(Me, W->GetSpellRange()))
				{
					W->CastOnPlayer();
				}
				else if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
				{
					CastQ(target);
				}
			}
		
			if (Contains(Args.Name_, "RivenIzunaBlade"))
			{
				if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
				{
					Q->CastOnPosition(target->GetPosition());
				}
			}
		}
		else
		{
			if (Contains(Args.Name_, "RivenIzunaBlade"))
			{
				if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
				{
					Q->CastOnPosition(target->GetPosition());
				}
			}
		
			if (Contains(Args.Name_, "RivenMartyr"))
			{
				if (comboR1->Enabled() && R->IsReady() && isRActive())
				{
					if (R2->CastOnPosition(target->GetPosition()))
					{
						if (Q->IsReady())
						{
							Q->CastOnPosition(target->GetPosition());
						}
					}
				}
			
				if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
				{
					CastQ(target);
				}
			}
		
			if (Contains(Args.Name_, "ItemTiamatCleave"))
			{
				if (W->IsReady() && target->IsValidTarget(Me, W->GetSpellRange()))
				{
					W->CastOnPlayer();
				}
				else if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
				{
					CastQ(target);
				}
			}
		}
	}
}

PLUGIN_EVENT(void) OnUpdate()
{
	if (GEntityList->Player()->GetSpellBook()->GetLevel(kSlotW) > 0)
	{
		W->SetSpellRange(Me->HasBuff("RivenFengShuiEngine") ? 330.0f : 260.0f);
	}

	if (Me->IsRecalling())
	{
		qStack = 0;
		return;
	}

	if (qStack != 0 && GGame->CurrentTick() - lastQTime > 3800)
	{
		qStack = 0;
	}

	if (keepQ && Me->HasBuff("RivenTriCleave"))
	{
		if (GBuffData->GetEndTime(Me->GetBuffDataByName("RivenTriCleave")) - GGame->Time() < 0.3)
		{
			Q->CastOnPosition(GGame->CursorPosition());
		}
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
	{
		Combo();
	}

	if (GetAsyncKeyState(burstKey->GetInteger()))
	{
		Burst();
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
	{
		Harass();
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
	{
		LaneClear();
		JungleClear();
	}

	if (GetAsyncKeyState(fleeKey->GetInteger()))
	{
		Flee();
	}
}

PLUGIN_EVENT(void) OnAfterAttack(IUnit* unit, IUnit* target)
{
	if (unit != Me)
	{
		return;
	}

	if (target == nullptr || target->IsDead())
	{
		return;
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
	{
		AfterCombo(target);
	}

	if (GetAsyncKeyState(burstKey->GetInteger()))
	{
		AfterBurst();
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeMixed)
	{
		AfterHarass(target);
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
	{
		AfterLaneClear(target);
		AfterJungleClear(target);
	}
}

PLUGIN_EVENT(void) OnDoCast(CastedSpell const& Args)
{
	if (Args.Caster_ != Me)
	{
		return;
	}

	if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
	{
		ComboCast(Args);
	}
	else if (GetAsyncKeyState(burstKey->GetInteger()))
	{
		BurstCast(Args);
	}
}

PLUGIN_EVENT(void) OnSpellCast(CastedSpell const& Args)
{
	if (Args.Caster_ == Me)
	{
		if ((Contains(Args.Name_, "RivenTriCleave") || Contains(Args.Name_, "ItemTiamatCleave"))&&
			(GOrbwalking->GetOrbwalkingMode() != kModeNone || GetAsyncKeyState(burstKey->GetInteger())))
		{
			GOrbwalking->ResetAA();
		}
		
		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, 600.0f);

			if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me) )
				return;

			if (target->IsValidTarget(Me, 600.0f))
			{
				if (Contains(Args.Name_, "ItemTiamatCleave"))
				{
					if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
					{
						CastQ(target);
					}
				}

				if (Contains(Args.Name_, "RivenFeint"))
				{
					if (comboR1->Enabled() && R->IsReady() && !isRActive() && target->IsValidTarget(Me, 500.0f))
					{
						R1Logic(target);
					}
				}
			}
		}

		if (GetAsyncKeyState(burstKey->GetInteger()))
		{
			auto target = GTargetSelector->GetFocusedTarget();

			if (target == nullptr || target->IsDead() || !target->IsHero() || !target->IsEnemy(Me) )
				return;

			if (target->IsValidTarget(Me, 600.0f))
			{
				if (Contains(Args.Name_, "RivenFeint"))
				{
					if (R->IsReady() && !isRActive())
					{
						R->CastOnPlayer();
					}
				}
			
				if (Contains(Args.Name_, "ItemTiamatCleave"))
				{
					if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
					{
						CastQ(target);
					}
				}
			
				if (Contains(Args.Name_, "RivenMartyr"))
				{
					if (Q->IsReady() && target->IsValidTarget(Me, 400.0f))
					{
						CastQ(target);
					}
				}
			}
		}
	}
	else if (Args.Caster_->IsHero() && Args.Caster_->IsEnemy(Me) && Args.Target_ == Me)
	{
		if (!eShield->Enabled() || !E->IsReady())
		{
			return;
		}

		if (find(dodgeSpell.begin(), dodgeSpell.end(), Args.Name_) != dodgeSpell.end())
		{
			E->CastOnPosition(GGame->CursorPosition());
		}
	}
}

PLUGIN_EVENT(void) OnGapcloser(GapCloserSpell const& Args)
{
	if (!antiGap->Enabled() || !W->IsReady() || Args.Source == nullptr || !Args.Source->IsHero() || !Args.Source->IsEnemy(Me))
	{
		return;
	}

	if (Args.Source->IsValidTarget(Me, W->GetSpellRange()) || DistanceToPlayer(Args.EndPosition) <= W->GetSpellRange())
	{
		W->CastOnPlayer();
	}
}

PLUGIN_EVENT(void) OnInterrupt(InterruptibleSpell const& Args)
{
	if (!interr->Enabled() || !W->IsReady() || Args.Source == nullptr || !Args.Source->IsHero() || !Args.Source->IsEnemy(Me))
	{
		return;
	}

	if (Args.Source->IsValidTarget(Me, W->GetSpellRange()) && Args.DangerLevel == kHighDanger)
	{
		W->CastOnPlayer();
	}
}

PLUGIN_EVENT(void) OnPlayAnimation(IUnit* source, std::string const Args)
{
	if (source != Me)
	{
		return;
	}

	if (Contains(Args, "c29a362b"))
	{
		qStack = 1;
		lastQTime = GGame->CurrentTick();
		Q1Q2Cancel();
	}

	if (Contains(Args, "c39a37be"))
	{
		qStack = 2;
		lastQTime = GGame->CurrentTick();
		Q1Q2Cancel();
	}

	if (Contains(Args, "c49a3951"))
	{
		qStack = 0;
		lastQTime = GGame->CurrentTick();
		Q3Cancel();
	}

	if (Contains(Args, "b5f63d21"))
	{
		WCancel();
	}

	if (Contains(Args, "bea6fbd2"))
	{
		R1Cancel();
	}

	if (Contains(Args, "bda6fa3f"))
	{
		R2Cancel();
	}
	
	// Spell E
	//if (Contains(Args, "b4f63b8e"))
}

PLUGIN_API void OnLoad(IPluginSDK* TakoRivenPlus)
{
	PluginSDKSetup(TakoRivenPlus);

	Me = GEntityList->Player();

	InitSpells();
	InitMenu();

	GEventManager->AddEventHandler(kEventOnPlayAnimation, OnPlayAnimation);
	GEventManager->AddEventHandler(kEventOnGameUpdate, OnUpdate);
	GEventManager->AddEventHandler(kEventOrbwalkAfterAttack, OnAfterAttack);
	GEventManager->AddEventHandler(kEventOnDoCast, OnDoCast);
	GEventManager->AddEventHandler(kEventOnSpellCast, OnSpellCast);
	GEventManager->AddEventHandler(kEventOnGapCloser, OnGapcloser);
	GEventManager->AddEventHandler(kEventOnInterruptible, OnInterrupt);
}

PLUGIN_API void OnUnload()
{
	mainMenu->Remove();

	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnUpdate);
	GEventManager->RemoveEventHandler(kEventOrbwalkAfterAttack, OnAfterAttack);
	GEventManager->RemoveEventHandler(kEventOnDoCast, OnDoCast);
	GEventManager->RemoveEventHandler(kEventOnSpellCast, OnSpellCast);
	GEventManager->RemoveEventHandler(kEventOnGapCloser, OnGapcloser);
	GEventManager->RemoveEventHandler(kEventOnInterruptible, OnInterrupt);
	GEventManager->RemoveEventHandler(kEventOnPlayAnimation, OnPlayAnimation);
}