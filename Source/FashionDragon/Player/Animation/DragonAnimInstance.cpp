#include "DragonAnimInstance.h"

#include <string>

#include "ControlledLeg.h"
#include "FashionDragon/DebugTools/QuickDebug.h"
#include "Poses/DragonIdlePose.h"
#include "Poses/DragonTrotPose.h"
#include "Poses/DragonWalkPose.h"
#include "Poses/DragonJumpPose.h"

/**
 * @brief Init
 */
void UDragonAnimInstance::NativeInitializeAnimation()
{
	LegPositions = TArray<FVector>();
	LegRotations = TArray<FRotator>();
	for (int i = 0; i < 4; i++)
	{
		LegPositions.Add(FVector(0.0f, 0.0f, 0.0f));
		LegRotations.Add(FRotator(0.0f, 0.0f, 0.0f));
	}
	
	ControlledLegs = TArray<FControlledLeg*>();
	// BackLeftPaw
	ControlledLegs.Add(new FControlledLeg(this, 0));
	// BackRightPaw
	ControlledLegs.Add(new FControlledLeg(this, 1));

	StateMachine = new FDragonAnimStateMachine(
		new FDragonIdlePose(this, ControlledLegs[0], ControlledLegs[1]),
		new FDragonWalkPose(this, ControlledLegs[0], ControlledLegs[1]),
		new FDragonTrotPose(this, ControlledLegs[0], ControlledLegs[1]),
		new FDragonJumpPose(this, ControlledLegs[0], ControlledLegs[1])
	);
}

/**
 * @brief Every frame, update the animation state and leg positions.
 */
void UDragonAnimInstance::NativeUpdateAnimation(const float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	const auto OwningActor = Cast<AMainCharacter>(GetOwningActor());
	if (!OwningActor)
		return;
	
	StateMachine->Tick(DeltaTime, OwningActor);
	UpdateWalkingBobCycle();

	for (int i = 0; i < ControlledLegs.Num(); i++)
	{
		const auto Leg = ControlledLegs[i];

		auto CumulativeEffector = FPoseEffector(Leg->Position, Leg->Rotation);
		for (const auto PoseDriver : StateMachine->PoseDrivers)
		{
			CumulativeEffector = PoseDriver->ToLegEffector(CumulativeEffector, ControlledLegs[i]);
		}

		// Merge the positions and rotations of all effectors
		Leg->Position = CumulativeEffector.Position;
		Leg->Rotation = CumulativeEffector.Rotation;
		LegPositions[i] = Leg->Position;
		LegRotations[i] = Leg->Rotation;
	}
}

/**
 * @brief Update the walking bob cycle based on the leg's position.
 */
void UDragonAnimInstance::UpdateWalkingBobCycle()
{
	// TODO: Rewrite bobbing
	// if (AnimationState != Walking)
	// {
	// 	WalkingBobCycle = 0.0f;
	// 	return;
	// }
	//
	// FControlledLeg* Leg;
	// if (ControlledLegs[1]->GetState() == ELegWalkingState::Planted)
	// 	Leg = ControlledLegs[1];
	// else if (ControlledLegs[0]->GetState() == ELegWalkingState::Planted)
	// 	Leg = ControlledLegs[0];
	// else
	// {
	// 	return;
	// }
	//
	// const auto OffsetFromOrigin = std::min(1.0, Leg->Position.Size() / 300.0f);
	// // Print offset
	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red,
	// 	FString::Printf(TEXT("Offset: %s"), *FString::SanitizeFloat(OffsetFromOrigin)));
	//
	// WalkingBobCycle = (1.0f - OffsetFromOrigin * 2.0f) * 15.0f;
}
