#pragma once

#include "CoreMinimal.h"
#include "Limbs/ControlledLeg.h"
#include "DragonAnimStateMachine.h"
#include "Adapters/DragonWingPoseAdapter.h"
#include "Animation/AnimInstance.h"
#include "Structs/ControlledBoneGroup.h"
#include "DragonAnimInstance.generated.h"

class AMainCharacter;

UCLASS()
class FASHIONDRAGON_API UDragonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	virtual void NativeBeginPlay() override; 

	void SetPhysicalBoneOffset(FName ParentBone, FName ChildName, const FVector& Position, const FRotator& Rotation) const;

public:
	FDragonAnimStateMachine* StateMachine;

	// Bone groups
	TFControlledBoneGroup<FControlledBone> ControlledRoot;
	TFControlledBoneGroup<FControlledBone> ControlledHead;
	TFControlledBoneGroup<FControlledBone> ControlledBody;
	TFControlledBoneGroup<FControlledBone> ControlledHips;
	TFControlledBoneGroup<FControlledLeg> BackLeftLeg;
	TFControlledBoneGroup<FControlledLeg> BackRightLeg;
	TFControlledBoneGroup<FControlledWing> LeftWing;
	TFControlledBoneGroup<FControlledWing> RightWing;

	// Aggregates
	TArray<TFControlledBoneGroup<FControlledLeg>*> ControlledLegs;
	TArray<TFControlledBoneGroup<FControlledWing>*> ControlledWings;

	// Adapters
	FDragonWingPoseAdapter* WingPoseAdapter;

	// IK Rig outputs
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	FTransform HeadTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	FTransform UpperNeckTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	FTransform LowerNeckTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	FTransform SpineFrontTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	FTransform SpineBackTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	FTransform HipTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	TArray<FVector> LegPositions;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	TArray<FRotator> LegRotations;

	AMainCharacter* GetCharacter() const;
};