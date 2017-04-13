#pragma once

#include "PluginSDK.h"
#include "RivenOption.h"

inline static void Q1Q2Cancel()
{
	if (GetAsyncKeyState(fleeKey->GetInteger()))
	{
		return;
	}

	if (manualCancel->Enabled() || GOrbwalking->GetOrbwalkingMode() != kModeNone || GetAsyncKeyState(burstKey->GetInteger()))
	{
		if (calculatePing->Enabled())
		{
			if (291 - GGame->Latency() > 0)
			{
				GPluginSDK->DelayFunctionCall(291 - GGame->Latency(), []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
					//GGame->IssueOrder(Me, kMoveTo, GGame->CursorPosition());

					if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() &&
						GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
					{
						GGame->IssueOrder(Me, kAttackTo, GOrbwalking->GetLastTarget());
						GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
					}
					//else
					//{
					//	GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
					//}
				});
				return;
			}
			else
			{
				GPluginSDK->DelayFunctionCall(1, []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
					//GGame->IssueOrder(Me, kMoveTo, GGame->CursorPosition());

					if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() &&
						GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
					{
						GGame->IssueOrder(Me, kAttackTo, GOrbwalking->GetLastTarget());
						GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
					}
					//else
					//{
					//	GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
					//}
				});
				return;
			}
		}
		else
		{
			GPluginSDK->DelayFunctionCall(291, []()
			{
				GOrbwalking->ResetAA();
				GGame->Taunt(kDance);
				//GGame->IssueOrder(Me, kMoveTo, GGame->CursorPosition());

				if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() &&
					GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
				{
					GGame->IssueOrder(Me, kAttackTo, GOrbwalking->GetLastTarget());
					GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
				}
				//else
				//{
				//	GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
				//}
			});
			return;
		}
	}
}

inline static void Q3Cancel()
{
	if (GetAsyncKeyState(fleeKey->GetInteger()))
	{
		return;
	}

	if (manualCancel->Enabled() || GOrbwalking->GetOrbwalkingMode() != kModeNone || GetAsyncKeyState(burstKey->GetInteger()))
	{
		if (calculatePing->Enabled())
		{
			if (391 - GGame->Latency() > 0)
			{
				GPluginSDK->DelayFunctionCall(391 - GGame->Latency(), []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
					//GGame->IssueOrder(Me, kMoveTo, GGame->CursorPosition());

					if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() &&
						GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
					{
						GGame->IssueOrder(Me, kAttackTo, GOrbwalking->GetLastTarget());
						GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
					}
					//else
					//{
					//	GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
					//}
				});
				return;
			}
			else
			{
				GPluginSDK->DelayFunctionCall(1, []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);

					//GGame->IssueOrder(Me, kMoveTo, GGame->CursorPosition());

					if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() &&
						GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
					{
						GGame->IssueOrder(Me, kAttackTo, GOrbwalking->GetLastTarget());
						GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
					}
					//else
					//{
					//	GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
					//}
				});
				return;
			}
		}
		else
		{
			GPluginSDK->DelayFunctionCall(391, []()
			{
				GOrbwalking->ResetAA();
				GGame->Taunt(kDance);
				//GGame->IssueOrder(Me, kMoveTo, GGame->CursorPosition());

				if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() &&
					GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
				{
					GGame->IssueOrder(Me, kAttackTo, GOrbwalking->GetLastTarget());
					GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
				}
				//else
				//{
				//	GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
				//}
			});
			return;
		}
	}
}

inline static void WCancel()
{
	if (GetAsyncKeyState(fleeKey->GetInteger()))
	{
		return;
	}

	if (manualCancel->Enabled() || GOrbwalking->GetOrbwalkingMode() != kModeNone || GetAsyncKeyState(burstKey->GetInteger()))
	{
		if (calculatePing->Enabled())
		{
			if (50 - GGame->Latency() > 0)
			{
				GPluginSDK->DelayFunctionCall(50 - GGame->Latency(), []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
				});
				return;
			}
			else
			{
				GPluginSDK->DelayFunctionCall(1, []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
				});
				return;
			}
		}
		else
		{
			GPluginSDK->DelayFunctionCall(50, []()
			{
				GOrbwalking->ResetAA();
				GGame->Taunt(kDance);
			});
			return;
		}
	}
}

inline static void R1Cancel()
{
	if (GetAsyncKeyState(fleeKey->GetInteger()))
	{
		return;
	}

	if (manualCancel->Enabled() || GOrbwalking->GetOrbwalkingMode() != kModeNone || GetAsyncKeyState(burstKey->GetInteger()))
	{
		if (calculatePing->Enabled())
		{
			if (50 - GGame->Latency() > 0)
			{
				GPluginSDK->DelayFunctionCall(50 - GGame->Latency(), []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
					//GGame->IssueOrder(Me, kMoveTo, GGame->CursorPosition());

					if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() &&
						GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
					{
						GGame->IssueOrder(Me, kAttackTo, GOrbwalking->GetLastTarget());
						GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
					}
					//else
					//{
					//	GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
					//}
				});
				return;
			}
			else
			{
				GPluginSDK->DelayFunctionCall(1, []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
					//GGame->IssueOrder(Me, kMoveTo, GGame->CursorPosition());

					if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() &&
						GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
					{
						GGame->IssueOrder(Me, kAttackTo, GOrbwalking->GetLastTarget());
						GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
					}
					//else
					//{
					//	GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
					//}
				});
				return;
			}
		}
		else
		{
			GPluginSDK->DelayFunctionCall(50, []()
			{
				GOrbwalking->ResetAA();
				GGame->Taunt(kDance);
				//GGame->IssueOrder(Me, kMoveTo, GGame->CursorPosition());

				if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() &&
					GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
				{
					GGame->IssueOrder(Me, kAttackTo, GOrbwalking->GetLastTarget());
					GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
				}
				//else
				//{
				//	GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
				//}
			});
			return;
		}
	}
}

inline static void R2Cancel()
{
	if (GetAsyncKeyState(fleeKey->GetInteger()))
	{
		return;
	}

	if (manualCancel->Enabled() || GOrbwalking->GetOrbwalkingMode() != kModeNone || GetAsyncKeyState(burstKey->GetInteger()))
	{
		if (calculatePing->Enabled())
		{
			if (180 - GGame->Latency() > 0)
			{
				GPluginSDK->DelayFunctionCall(180 - GGame->Latency(), []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
					//GGame->IssueOrder(Me, kMoveTo, GGame->CursorPosition());

					if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() &&
						GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
					{
						GGame->IssueOrder(Me, kAttackTo, GOrbwalking->GetLastTarget());
						GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
					}
					//else
					//{
					//	GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
					//}
				});
				return;
			}
			else
			{
				GPluginSDK->DelayFunctionCall(1, []()
				{
					GOrbwalking->ResetAA();
					GGame->Taunt(kDance);
					//GGame->IssueOrder(Me, kMoveTo, GGame->CursorPosition());

					if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() &&
						GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
					{
						GGame->IssueOrder(Me, kAttackTo, GOrbwalking->GetLastTarget());
						GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
					}
					//else
					//{
					//	GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
					//}
				});
				return;
			}
		}
		else
		{
			GPluginSDK->DelayFunctionCall(180, []()
			{
				GOrbwalking->ResetAA();
				GGame->Taunt(kDance);
				//GGame->IssueOrder(Me, kMoveTo, GGame->CursorPosition());

				if (GOrbwalking->GetLastTarget() != nullptr && !GOrbwalking->GetLastTarget()->IsDead() &&
					GOrbwalking->GetLastTarget()->IsValidTarget(Me, Me->GetRealAutoAttackRange(GOrbwalking->GetLastTarget()) + Me->BoundingRadius()))
				{
					GGame->IssueOrder(Me, kAttackTo, GOrbwalking->GetLastTarget());
					GOrbwalking->Orbwalk(GOrbwalking->GetLastTarget(), GGame->CursorPosition());
				}
				//else
				//{
				//	GOrbwalking->Orbwalk(nullptr, GGame->CursorPosition());
				//}
			});
			return;
		}
	}
}