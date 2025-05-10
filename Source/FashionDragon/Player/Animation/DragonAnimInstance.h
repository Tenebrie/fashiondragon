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

	void UpdateWalkingBobCycle();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	float WalkingBobCycle;

public:
	FDragonAnimStateMachine* StateMachine;
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