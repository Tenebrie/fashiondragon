#pragma once
#include "FashionDragon/Player/Animation/Enums/LegIdleState.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(
	FDragonIdleLegDriverStateChangedDelegate,
	ELegIdleState OldState,
	ELegIdleState NewState
);
