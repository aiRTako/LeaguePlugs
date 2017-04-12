#include "PluginSDK.h"
#include "Extensions.h"
#include <map>
#include <string>

using namespace std;

PluginSetup("Tako Lucian Plus");

IUnit* Me;

ISpell2* Q;
ISpell2* QExtend;
ISpell2* W;
ISpell2* E;
ISpell2* R;

IInventoryItem* Cutlass;
IInventoryItem* Botrk;
IInventoryItem* Youmuus;

IMenu* mainMenu;

IMenu* comboMenu;
IMenuOption* comboQ;
IMenuOption* comboQExtend;
IMenuOption* comboW;
IMenuOption* comboEDash;
IMenuOption* comboEReset;
IMenuOption* comboETurret;
IMenuOption* comboESafe;
IMenuOption* comboR;

IMenu* harassMenu;
IMenuOption* harassQ;
IMenuOption* harassQExtend;
IMenuOption* harassW;
IMenuOption* harassMana;

IMenu* laneClearMenu;
IMenuOption* laneClearQ;
IMenuOption* laneClearW;
IMenuOption* laneClearMana;

IMenu* jungleClearMenu;
IMenuOption* jungleClearQ;
IMenuOption* jungleClearW;
IMenuOption* jungleClearE;
IMenuOption* jungleClearMana;

IMenu* killStealMenu;
IMenuOption* killStealQ;
IMenuOption* killStealW;

IMenu* miscMenu;
IMenuOption* autoHarass;
IMenuOption* antiMelee;
std::map<int, IMenuOption*> AntiTarget;

IMenu* drawMenu;
IMenuOption* drawQ;
IMenuOption* drawQExtend;
IMenuOption* drawW;
IMenuOption* drawR;

void InitSpell()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kTargetCast, false, true, kCollidesWithNothing);
	Q->SetOverrideDelay(0.25f);
	Q->SetOverrideRange(650.0f);

	QExtend = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, false, false, kCollidesWithNothing);
	QExtend->SetSkillshot(0.35f, 25.0f, 1800.0f, 900.0f);

	W = GPluginSDK->CreateSpell2(kSlotW, kLineCast, true, true, kCollidesWithMinions);
	W->SetSkillshot(0.3f, 80.0f, 1600.f, 1000.0f);

	E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, false, false, kCollidesWithNothing);

	R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, true, false, kCollidesWithMinions);
	R->SetSkillshot(0.1f, 110.0f, 2800.0f, 1200.0f);

	Cutlass = GPluginSDK->CreateItemForId(3144, 550);
	Botrk = GPluginSDK->CreateItemForId(3153, 550);
	Youmuus = GPluginSDK->CreateItemForId(3142, 0);
}

void InitMenu()
{
	mainMenu = GPluginSDK->AddMenu("Tako Lucian Plus");

	comboMenu = mainMenu->AddMenu("Combo Settings");
	{
		comboQ = comboMenu->CheckBox("Use Q", true);
		comboQExtend = comboMenu->CheckBox("Use Q Extend", true);
		comboW = comboMenu->CheckBox("Use W", true);
		comboEDash = comboMenu->CheckBox("Use E Dash To Target", true);
		comboEReset = comboMenu->CheckBox("Use E Reset Auto Attack", true);
		comboETurret = comboMenu->CheckBox("Use E Under Enemy Turret(or Dash to Enemy Turret)", false);
		comboESafe = comboMenu->CheckBox("Use E Safe Check", true);
		comboR = comboMenu->CheckBox("Use R(Only KillSteal Target)", true);
	}

	harassMenu = mainMenu->AddMenu("Harass Settings");
	{
		harassQ = harassMenu->CheckBox("Use Q", true);
		harassQExtend = harassMenu->CheckBox("Use Q Extend", true);
		harassW = harassMenu->CheckBox("Use W", false);
		harassMana = harassMenu->AddFloat("When Player ManaPercent >= x%", 0, 100, 60);
	}

	//laneClearMenu = mainMenu->AddMenu("LaneClear Settings");
	//{
	//	laneClearQ = laneClearMenu->CheckBox("Use Q", true);
	//	laneClearW = laneClearMenu->CheckBox("Use W", false);
	//	laneClearMana = laneClearMenu->AddFloat("When Player ManaPercent >= x%", 0, 100, 60);
	//}
	
	jungleClearMenu = mainMenu->AddMenu("JungleClear Settings");
	{
		jungleClearQ = jungleClearMenu->CheckBox("Use Q", true);
		jungleClearW = jungleClearMenu->CheckBox("Use W", true);
		jungleClearE = jungleClearMenu->CheckBox("Use E", true);
		jungleClearMana = jungleClearMenu->AddFloat("When Player ManaPercent >= x%", 0, 100, 20);
	}

	killStealMenu = mainMenu->AddMenu("KillSteal Settings");
	{
		killStealQ = killStealMenu->CheckBox("Use Q", true);
		killStealW = killStealMenu->CheckBox("Use W", true);
	}

	miscMenu = mainMenu->AddMenu("Misc Settings");
	{
		autoHarass = miscMenu->CheckBox("Auto Harass in LaneClear Mode", true);
		antiMelee = miscMenu->CheckBox("Use E Anti Melee", true);
		for (auto target : GEntityList->GetAllHeros(false, true))
		{
			if (target != nullptr)
			{
				std::string szMenuName = "Anti Gapcloser Target: " + std::string(target->ChampionName());
				AntiTarget[target->GetNetworkId()] = miscMenu->CheckBox(szMenuName.c_str(), target->IsMelee());
			}
		}
	}

	drawMenu = mainMenu->AddMenu("Draw Settings");
	{
		drawQ = drawMenu->CheckBox("Draw Q Range", false);
		drawQExtend = drawMenu->CheckBox("Draw QExtend Range", false);
		drawW = drawMenu->CheckBox("Draw W Range", false);
		drawR = drawMenu->CheckBox("Draw R Range", false);
	}
}

void CastQLogic(IUnit* target)
{
	if (!Q->IsReady() || target == nullptr || !Me->IsValidTarget(target, QExtend->Range()) || Me->IsDashing())
	{
		return;
	}

	if (Me->IsValidTarget(target, Q->Range()))
	{
		Q->CastOnUnit(target);
	}
	else if (Me->IsValidTarget(target, QExtend->Range()))
	{
		AdvPredictionOutput Pred;
		QExtend->RunPrediction(target, false, kCollidesWithNothing, &Pred);

		if (Pred.HitChance >= kHitChanceHigh)
		{
			for (auto minion : GEntityList->GetAllMinions(false, true, true))
			{
				if (minion == nullptr || !Me->IsValidTarget(minion, Q->Range()))
				{
					continue;
				}

				if (Distance(Pred.CastPosition, Me->GetPosition().Extend(minion->GetPosition(), Distance(Me, Pred.CastPosition))) < 25)
				{
					Q->CastOnUnit(minion);
					return;
				}
			}
		}
	}
}

void BeforeELogic(IUnit* target)
{
	if (!E->IsReady() || 
		target == nullptr || 
		Me->IsValidTarget(target, Me->GetRealAutoAttackRange(target)) || 
		!Me->IsValidTarget(target, Me->AttackRange() + Me->BoundingRadius() + 475.0f))
	{
		return;
	}

	auto EPos = Me->GetPosition().Extend(GGame->CursorPosition(), 475.0f);

	if (GUtility->IsPositionUnderTurret(EPos) && comboETurret->Enabled())
	{
		return;
	}

	if (GetEnemiesCount(EPos, 500) >= 3 && GetAlliesCount(EPos, 400) < 3 && comboESafe->Enabled())
	{
		return;
	}

	/*
	if (Distance(Me, target->GetPosition()) >= Me->AttackRange() + Me->BoundingRadius() &&
		GetEnemiesCount(Me->ServerPosition().Extend(GGame->CursorPosition(), 475.0f), 1000) < 3 &&
		Distance(Me->ServerPosition().Extend(GGame->CursorPosition(), 475.0f), target) <= Me->AttackRange() + Me->BoundingRadius())
	*/
	if (Distance(Me, GGame->CursorPosition()) >= Me->AttackRange() + Me->BoundingRadius() &&
		GetEnemiesCount(Me->ServerPosition().Extend(GGame->CursorPosition(), 475.0f), 1000) < 3 &&
		Distance(target, Me->ServerPosition().Extend(GGame->CursorPosition(), 475.0f)) <= Me->AttackRange() + Me->BoundingRadius())
	{
		E->CastOnPosition(Me->ServerPosition().Extend(GGame->CursorPosition(), 475.0f));
	}
}

void AfterELogic(IUnit* target)
{
	if (!E->IsReady() || target == nullptr)
	{
		return;
	}

	Vec3 EPos;

    //if (Distance(Me, target->GetPosition()) <= Me->AttackRange() + Me->BoundingRadius()) More Smart to Judge the CursorPosition
	if (Distance(Me, GGame->CursorPosition()) <= Me->AttackRange() + Me->BoundingRadius())
	{
		EPos = Me->ServerPosition().Extend(GGame->CursorPosition(), 150.0f);
	}
	else
	{
		EPos = Me->ServerPosition().Extend(GGame->CursorPosition(), 475.0f);
	}

	if (GUtility->IsPositionUnderTurret(EPos) && comboETurret->Enabled())
	{
		return;
	}

	if (GetEnemiesCount(EPos, 500) >= 3 && GetAlliesCount(EPos, 400) < 3 && comboESafe->Enabled())
	{
		return;
	}

	E->CastOnPosition(EPos);
}

void OnUpdateKillSteal()
{
	if (killStealQ->Enabled() && Q->IsReady())
	{
		for (auto target : GEntityList->GetAllHeros(false, true))
		{
			if (target == nullptr || !Me->IsValidTarget(target, QExtend->Range()) || target->IsInvulnerable() || IsUnKillable(target))
			{
				return;
			}

			auto DMG = GDamage->GetSpellDamage(Me, target, kSlotQ);

			if (DMG >= target->GetHealth())
			{
				CastQLogic(target);
				return;
			}
		}
	}

	if (killStealW->Enabled() && W->IsReady())
	{
		for (auto target : GEntityList->GetAllHeros(false, true))
		{
			if (target == nullptr || !Me->IsValidTarget(target, W->Range()) || target->IsInvulnerable() || IsUnKillable(target))
			{
				return;
			}

			auto DMG = GDamage->GetSpellDamage(Me, target, kSlotW);

			if (DMG >= target->GetHealth())
			{
				W->CastOnTarget(target, kHitChanceVeryHigh);
				return;
			}
		}
	}
}

void OnUpdateCombo()
{
	auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, QExtend->Range());

	if (target != nullptr && Me->IsValidTarget(target, QExtend->Range()) && !target->IsInvulnerable())
	{
		if (Cutlass->IsOwned() && Cutlass->IsReady() && Me->IsValidTarget(target, 600.0f))
		{
			Cutlass->CastOnTarget(target);
		}

		if (Botrk->IsOwned() && Botrk->IsReady() && Me->IsValidTarget(target, 600.0f))
		{
			Botrk->CastOnTarget(target);
		}

		if (comboEDash->Enabled() && 
			E->IsReady() && 
			Me->IsValidTarget(target, Me->GetRealAutoAttackRange(target) + 475.0f) && 
			!Me->IsValidTarget(target, Me->GetRealAutoAttackRange(target)))
		{
			BeforeELogic(target);
		}

		if (comboQExtend->Enabled() && 
			QExtend->IsReady() && 
			Me->IsValidTarget(target, QExtend->Range()) && 
			!Me->IsValidTarget(target, Q->Range()) && 
			!Me->HasBuff("lucianpassivebuff"))
		{
			CastQLogic(target);
		}

		if (comboR->Enabled() &&
			R->IsReady() && 
			Me->IsValidTarget(target, R->Range()) &&
			!Me->IsValidTarget(target, Me->AttackRange() + Me->BoundingRadius()) &&
			!IsUnKillable(target) &&
			!target->IsInvulnerable() &&
			GetEnemiesCount(target, 500) <= 1)
		{
			auto rDMG = GDamage->GetSpellDamage(Me, target, kSlotR) * (10 + 5*Me->GetSpellLevel(3));
			auto tDis = Distance(Me, target);

			if (rDMG * 0.8 > target->GetHealth() && tDis < 700 && !Q->IsReady())
			{
				R->CastOnTarget(target);
			}
			else if (rDMG * 0.6 >target->GetHealth() && tDis < 900)
			{
				R->CastOnTarget(target);
			}
			else if (rDMG * 0.4 >target->GetHealth() && tDis < 1200)
			{
				R->CastOnTarget(target);
			}
		}
	}
}

void OnUpdateHarass()
{
	if (Me->ManaPercent() >= harassMana->GetFloat())
	{
		if (harassQ->Enabled() && Q->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, Q->Range());

			if (target != nullptr && Me->IsValidTarget(target, Q->Range()) && !target->IsInvulnerable())
			{
				CastQLogic(target);
			}
		}

		if (harassQExtend->Enabled() && QExtend->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, PhysicalDamage, QExtend->Range());

			if (target != nullptr &&
				Me->IsValidTarget(target, QExtend->Range()) && 
				!Me->IsValidTarget(target, Q->Range()) && 
				!target->IsInvulnerable())
			{
				CastQLogic(target);
			}
		}

		if (harassW->Enabled() && W->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, W->Range());

			if (target != nullptr && Me->IsValidTarget(target, W->Range()) && !target->IsInvulnerable())
			{
				W->CastOnTarget(target, kHitChanceVeryHigh);
			}
		}
	}
}

void OnUpdateFarm()
{
	if (autoHarass->Enabled())
	{
		OnUpdateHarass();
	}

	//OnUpdateLaneClear();
}

/*
void OnUpdateLaneClear()
{
	if (Me->ManaPercent() >= laneClearMana->GetFloat())
	{
		if (laneClearQ->Enabled() && Q->IsReady())
		{
			for (auto minion : GEntityList->GetAllMinions(false, true, false))
			{
				AdvPredictionOutput Pred;
				Q->RunPrediction(minion, true, kCollidesWithNothing, &Pred);
			}
		}

		if (laneClearW->Enabled() && W->IsReady())
		{
		    for (auto minion : GEntityList->GetAllMinions(false, true, false))
			{
			     
			}
		}
	}
}
*/

void OnAfterAttackCombo(IUnit* target)
{
	if (target != nullptr && Me->IsValidTarget(target, QExtend->Range()) && !target->IsInvulnerable())
	{
		if (Youmuus->IsOwned() && Youmuus->IsReady())
		{
			Youmuus->CastOnPlayer();
		}

		if (comboEReset && E->IsReady())
		{
			AfterELogic(target);
		}
		else if (comboQ && Q->IsReady() && Me->IsValidTarget(target, Q->Range()))
		{
			Q->CastOnUnit(target);
		}
		else if (comboW && W->IsReady() && Me->IsValidTarget(target, W->Range()))
		{
			W->CastOnPosition(target->GetPosition());
		}
	}
}

/*
void OnAfterAttackLane(IUnit* target)
{
   // if target is Turret/HQ/Barrack
   // use E/W
   // W cast to Cursor Pos
   // E cast to min dash
}
*/

void OnAfterAttackJungle()
{
	if (Me->ManaPercent() >= jungleClearMana->GetFloat())
	{
		for (auto mob : GEntityList->GetAllMinions(false, false, true))
		{
			if (mob == nullptr || !Me->IsValidTarget(mob, Q->Range()))
			{
				continue;
			}

			if (jungleClearE->Enabled() && E->IsReady())
			{
				E->CastOnPosition(Me->GetPosition().Extend(GGame->CursorPosition(), 150));
			}
			else if (jungleClearQ->Enabled() && Q->IsReady())
			{
				Q->CastOnUnit(mob);
			}
			else if (jungleClearW->Enabled() && W->IsReady())
			{
				W->CastOnPosition(mob->GetPosition());
			}
		}
	}
}

PLUGIN_EVENT(void) OnUpdate()
{
	if (Me->IsDead() || Me->IsRecalling() || Me->IsWindingUp())
	{
		return;
	}

	if (Me->HasBuff("LucianR"))
	{
		GOrbwalking->SetAttacksAllowed(false);
		return;
	}

	GOrbwalking->SetAttacksAllowed(true);

	OnUpdateKillSteal();

	switch (GOrbwalking->GetOrbwalkingMode())
	{
	case kModeCombo:
		OnUpdateCombo();
		break;
	case kModeMixed:
		OnUpdateHarass();
		break;
	case kModeLaneClear:
		OnUpdateFarm();
		break;
	default:
		break;
	}
}

PLUGIN_EVENT(void) OnAfterAttack(IUnit* source, IUnit* target)
{
	if (source != Me || target == nullptr)
	{
		return;
	}

	switch (GOrbwalking->GetOrbwalkingMode())
	{
	case kModeCombo:
		if (target->IsHero() && target->GetTeam() != Me->GetTeam())
		{
			OnAfterAttackCombo(target);
		}
	case kModeLaneClear:
		OnAfterAttackJungle();
		break;
	default:
		break;
	}
}

PLUGIN_EVENT(void) OnGapCloser(GapCloserSpell const& Args)
{
	if (!Args.Source->IsEnemy(Me) || !Args.Source->IsHero() || !E->IsReady())
	{
		return;
	}

	if (AntiTarget[Args.Source->GetNetworkId()]->Enabled())
	{
		if (Me->IsValidTarget(Args.Source, 250) || Distance(Args.EndPosition, Me->GetPosition()) <= 250)
		{
			E->CastOnPosition(Me->GetPosition().Extend(Args.Source->GetPosition(), -475.0f));
		}
	}
}

PLUGIN_EVENT(void) OnSpellCast(CastedSpell const& Args)
{
	if (Args.Caster_->IsEnemy(Me) &&
		Args.Caster_->IsHero() && 
		Args.Caster_->IsMelee() && 
		antiMelee->Enabled() && 
		Me->HealthPercent() <= 50 &&
		E->IsReady() &&
		Args.AutoAttack_ && 
		Args.Target_ == Me)
	{
		E->CastOnPosition(Me->GetPosition().Extend(Args.Caster_->GetPosition(), -475.0f));
	}
	
	if (Args.Caster_ != Me)
	{
		return;
	}

	if (Contains(Args.Name_, "LucianE"))
	{
		GOrbwalking->ResetAA();
	}

	if (Contains(Args.Name_, "LucianR"))
	{
		if (Youmuus->IsOwned() && Youmuus->IsReady())
		{
			Youmuus->CastOnPlayer();
		}
	}
}

PLUGIN_EVENT(void) OnPlayAnimation(IUnit* source, std::string const Args)
{
	if (source != Me)
	{
		return;
	}

	//Q Animation 
	if (Contains(Args, "b2f63868"))
	{
		GGame->IssueOrder(Me, kMoveTo, GGame->CursorPosition());
		GPluginSDK->DelayFunctionCall(10, []() {GOrbwalking->ResetAA();});
	}

	//W Animation 
	if (Contains(Args, "b5f63d21"))
	{
		GGame->IssueOrder(Me, kMoveTo, GGame->CursorPosition());
		GPluginSDK->DelayFunctionCall(10, []() {GOrbwalking->ResetAA();});
	}
}

PLUGIN_EVENT(void) OnRender()
{
	if (Me->IsDead() || GGame->IsChatOpen() || GGame->IsScoreboardOpen() || GGame->IsShopOpen())
	{
		return;
	}
	
	if (Q->IsReady() && drawQ->Enabled()) 
	{
		GRender->DrawOutlinedCircle(Me->GetPosition(), Vec4(255, 255, 0, 255), Q->Range());
	}

	if (QExtend->IsReady() && drawQExtend->Enabled())
	{
		GRender->DrawOutlinedCircle(Me->GetPosition(), Vec4(255, 255, 0, 255), QExtend->Range());
	}

	if (W->IsReady() && drawW->Enabled())
	{
		GRender->DrawOutlinedCircle(Me->GetPosition(), Vec4(255, 255, 0, 255), W->Range());
	}

	if (R->IsReady() && drawR->Enabled()) 
	{ 
		GRender->DrawOutlinedCircle(Me->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); 
	}
}


PLUGIN_API void OnLoad(IPluginSDK* TakoLucianPlus)
{
	PluginSDKSetup(TakoLucianPlus);

	Me = GEntityList->Player();

	InitSpell();
	InitMenu();

	GEventManager->AddEventHandler(kEventOnGameUpdate, OnUpdate);
	GEventManager->AddEventHandler(kEventOrbwalkAfterAttack, OnAfterAttack);
	GEventManager->AddEventHandler(kEventOnGapCloser, OnGapCloser);
	GEventManager->AddEventHandler(kEventOnSpellCast, OnSpellCast);
	GEventManager->AddEventHandler(kEventOnPlayAnimation, OnPlayAnimation);
	GEventManager->AddEventHandler(kEventOnRender, OnRender);
}

PLUGIN_API void OnUnload()
{
	mainMenu->Remove();

	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnUpdate);
	GEventManager->RemoveEventHandler(kEventOrbwalkAfterAttack, OnAfterAttack);
	GEventManager->RemoveEventHandler(kEventOnGapCloser, OnGapCloser);
	GEventManager->RemoveEventHandler(kEventOnSpellCast, OnSpellCast);
	GEventManager->RemoveEventHandler(kEventOnPlayAnimation, OnPlayAnimation);
	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
}

/*
virtual void OnGameUpdate() = 0;
virtual void OnAfterAttack(IUnit* Source, IUnit* Target) = 0;
virtual void OnBeforeAttack(IUnit* Target) = 0;
virtual void OnGapCloser(GapCloserSpell const& Args) = 0;
virtual void OnInterruptible(InterruptibleSpell const& Args) = 0;
virtual void OnDash(UnitDash* Args) = 0;
virtual void OnCreateObject(IUnit* Object) = 0;
virtual void OnDeleteObject(IUnit* Object) = 0;
virtual void OnLevelUp(IUnit* Source, int NewLevel) = 0;
virtual void OnExitVisible(IUnit* Source) = 0;
virtual void OnLoad() {};
virtual void OnBuffAdd(IUnit* Source, void* BuffData) = 0;
virtual void OnBuffRemove(IUnit* Source, void* BuffData) = 0;
virtual void OnRender() = 0;
virtual void OnGameEnd() = 0;
virtual void OnSpellCast(CastedSpell const& Args) = 0;
*/