#pragma once

#include "CoreMinimal.h"
#include "ControlledLeg.h"
#include "DragonAnimStateMachine.h"
#include "../MainCharacter.h"
#include "Animation/AnimInstance.h"
#include "DragonAnimInstance.generated.h"

UCLASS()
class FASHIONDRAGON_API UDragonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	TArray<FControlledLeg*> ControlledLegs;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	FRotator HipRotation;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	TArray<FVector> LegPositions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	TArray<FRotator> LegRotations;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	UCurveVector* BackLegMoveCurve;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	UCurveFloat* BackLegRotateCurve;
};