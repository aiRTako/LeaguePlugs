#include "PluginSDK.h"
#include "Extensions.h"
#include "VayneOption.h"

#include "OnUpdate.h"
#include "OnAfterAttack.h"
#include "OnSpellCast.h"
#include "OnGapcloser.h"
#include "OnInterrupt.h"
#include "OnCreateObj.h"
#include "OnRender.h"

PluginSetup("TakoVayne Plus");

PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
{
	PluginSDKSetup(PluginSDK);

	Me = GEntityList->Player();

	InitSpells();
	InitMenu();

	GEventManager->AddEventHandler(kEventOnGameUpdate, OnUpdate);
	//GEventManager->AddEventHandler(kEventOrbwalkBeforeAttack, OnBeforeAttack);
	GEventManager->AddEventHandler(kEventOrbwalkAfterAttack, OnAfterAttack);
	GEventManager->AddEventHandler(kEventOnSpellCast, OnSpellCast);
	GEventManager->AddEventHandler(kEventOnGapCloser, OnGapcloser);
	GEventManager->AddEventHandler(kEventOnInterruptible, OnInterrupt);
	GEventManager->AddEventHandler(kEventOnCreateObject, OnCreateObj);
	GEventManager->AddEventHandler(kEventOnRender, OnRender);
}

PLUGIN_API void OnUnload()
{
	mainMenu->Remove();

	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnUpdate);
	//GEventManager->RemoveEventHandler(kEventOrbwalkBeforeAttack, OnBeforeAttack);
	GEventManager->RemoveEventHandler(kEventOrbwalkAfterAttack, OnAfterAttack);
	GEventManager->RemoveEventHandler(kEventOnSpellCast, OnSpellCast);
	GEventManager->RemoveEventHandler(kEventOnGapCloser, OnGapcloser);
	GEventManager->RemoveEventHandler(kEventOnInterruptible, OnInterrupt);
	GEventManager->RemoveEventHandler(kEventOnCreateObject, OnCreateObj);
	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);
}