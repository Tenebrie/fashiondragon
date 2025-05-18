#pragma once

#include "CoreMinimal.h"
#include "Limbs/ControlledLeg.h"
#include "DragonAnimStateMachine.h"
#include "../MainCharacter.h"
#include "Adapters/DragonWingPoseAdapter.h"
#include "Animation/AnimInstance.h"
#include "DragonAnimInstance.generated.h"

UCLASS()
class FASHIONDRAGON_API UDragonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	TArray<FControlledLeg*> ControlledLegs;
	TArray<FControlledWing*> ControlledWings;

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	TMap<FName, FTransform> LastBoneOffsets;
	void SetBoneOffset(FName ParentBone, FName ChildName, const FVector& Position, const FRotator& Rotation) const;

	float WalkingBobCycle;

public:
	FDragonAnimStateMachine* StateMachine;
	FControlledBone* ControlledBody;
	FControlledBone* ControlledHips;
	FControlledLeg* BackLeftLeg;
	FControlledLeg* BackRightLeg;
	FControlledWing* LeftWing;
	FControlledWing* RightWing;

	FDragonWingPoseAdapter* WingPoseAdapter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	FRotator HipRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	TArray<FVector> LegPositions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	TArray<FRotator> LegRotations;
};