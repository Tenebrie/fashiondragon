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

	TMap<FName, FTransform> LastBoneOffsets;
	void SetBoneOffset(FName ParentBone, FName ChildName, const FVector& Position, const FRotator& Rotation) const;

public:
	FDragonAnimStateMachine* StateMachine;
	TFControlledBoneGroup<FControlledBone> ControlledBody;
	TFControlledBoneGroup<FControlledBone> ControlledHips;
	TArray<TFControlledBoneGroup<FControlledLeg>*> ControlledLegs;
	TArray<TFControlledBoneGroup<FControlledWing>*> ControlledWings;
	TFControlledBoneGroup<FControlledLeg> BackLeftLeg;
	TFControlledBoneGroup<FControlledLeg> BackRightLeg;
	TFControlledBoneGroup<FControlledWing> LeftWing;
	TFControlledBoneGroup<FControlledWing> RightWing;

	FDragonWingPoseAdapter* WingPoseAdapter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	TArray<FVector> LegPositions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	TArray<FRotator> LegRotations;

	AMainCharacter* GetCharacter() const;
};