#pragma once

#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

class FDragonRandomSwayDriverBody;

class FDragonRandomSwayPose final : public FProceduralPose
{
public:
	explicit FDragonRandomSwayPose(UDragonAnimInstance* Anim);

	// FDragonRandomSwayDriverBody* BodyDriver;
	// FDragonRandomSwayDriverBody* HipsDriver;
	// FDragonIdleLegDriver* LeftLegDriver;
	// FDragonIdleLegDriver* RightLegDriver;
	// FDragonIdleWingDriver* LeftWingDriver;
	// FDragonIdleWingDriver* RightWingDriver;
};