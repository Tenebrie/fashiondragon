#pragma once

#include "CoreMinimal.h"
#include "Limbs/ControlledLeg.h"
#include "DragonAnimStateMachine.h"
#include "Adapters/DragonWingPoseAdapter.h"
#include "Adapters/WingPose/BaseDragonWingPoseAdapter.h"
#include "Animation/AnimInstance.h"
#include "Structs/ControlledBoneGroup.h"
#include "DragonAnimInstance.generated.h"

class UBaseDragonWingPoseAdapter;
class FControlledWing;
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
	TUniquePtr<FDragonAnimStateMachine> StateMachine;

	// Bone groups
	TFControlledBoneGroup<FControlledBone> ControlledRoot;
	TFControlledBoneGroup<FControlledBone> ControlledHead;
	TFControlledBoneGroup<FControlledBone> ControlledBody;
	TFControlledBoneGroup<FControlledBone> ControlledHips;
	TFControlledBoneGroup<FControlledBone> LeftHand;
	TFControlledBoneGroup<FControlledBone> RightHand;
	TFControlledBoneGroup<FControlledLeg> BackLeftLeg;
	TFControlledBoneGroup<FControlledLeg> BackRightLeg;
	TFControlledBoneGroup<FControlledWing> LeftWing;
	TFControlledBoneGroup<FControlledWing> RightWing;

	// Aggregates
	TArray<TFControlledBoneGroup<FControlledBone>*> ControlledHands;
	TArray<TFControlledBoneGroup<FControlledLeg>*> ControlledLegs;
	TArray<TFControlledBoneGroup<FControlledWing>*> ControlledWings;

	// IK Rig outputs
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ProceduralAnimation")
	FTransform HeadTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ProceduralAnimation")
	FTransform UpperNeckTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ProceduralAnimation")
	FTransform LowerNeckTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ProceduralAnimation")
	FTransform SpineFrontTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ProceduralAnimation")
	FTransform SpineBackTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ProceduralAnimation")
	FTransform HipTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ProceduralAnimation")
	TArray<FTransform> HandTransforms;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ProceduralAnimation")
	TArray<FVector> LegPositions;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ProceduralAnimation")
	TArray<FRotator> LegRotations;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ProceduralAnimation")
	TArray<FPoseWingEffector> WingEffectors;

	UFUNCTION(BlueprintCallable) AMainCharacter* GetCharacter() const;
	TFControlledBoneGroup<FControlledBone>* GetBoneGroup(const FName& BoneName);
};