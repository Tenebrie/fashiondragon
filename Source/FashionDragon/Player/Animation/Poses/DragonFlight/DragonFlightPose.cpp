#include "DragonFlightPose.h"

#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/Animation/Enums/DriverLayer.h"

FDragonFlightPose::FDragonFlightPose(UDragonAnimInstance* Anim): FProceduralPose(Anim)
{
	LeftWingDriver = new FDragonFlightWingDriver(Anim, Anim->LeftWing.GetBone(EDriverLayer::Primary));
	RightWingDriver = new FDragonFlightWingDriver(Anim, Anim->RightWing.GetBone(EDriverLayer::Primary));
	WingDrivers = {
		LeftWingDriver,
		RightWingDriver,
	};
}

void FDragonFlightPose::ResetState()
{
	FProceduralPose::ResetState();
}
