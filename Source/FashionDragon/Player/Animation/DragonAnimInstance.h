#pragma once

#include "CoreMinimal.h"
#include "AbstractProceduralPose.h"
#include "ControlledLeg.h"
#include "../MainCharacter.h"
#include "Animation/AnimInstance.h"
#include "Poses/DragonIdlePose.h"
#include "Poses/DragonJumpPose.h"
#include "Poses/DragonTrotPose.h"
#include "Poses/DragonWalkPose.h"
#include "DragonAnimInstance.generated.h"

const enum EAnimationState
{
	Idle = 0,
	Walking = 1,
	Running = 3,
	Jumping = 4,
};


UCLASS()
class FASHIONDRAGON_API UDragonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	TArray<FControlledLeg*> ControlledLegs;
	TArray<FAbstractProceduralPose*> PoseDrivers;

	FDragonIdlePose* IdlePoseDriver;
	FDragonWalkPose* WalkPoseDriver;
	FDragonTrotPose* TrotPoseDriver;
	FDragonJumpPose* JumpPoseDriver;

	void BlendDrivers(float DeltaTime) const;
	void UpdateWalkingBobCycle();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	float WalkingBobCycle;

public:
	float AnimationLockout = 0.0f;
	
	EAnimationState AnimationState = Idle;
	void SetState(EAnimationState);
	
	float GetWalkingBobCycle() const { return WalkingBobCycle; }
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	TArray<FVector> LegPositions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	TArray<FRotator> LegRotations;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	UCurveVector* BackLegMoveCurve;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	UCurveFloat* BackLegRotateCurve;
};