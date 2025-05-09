	#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "Animation/AnimInstance.h"
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

private:
	class FControlledLeg;

	TArray<FControlledLeg*> ControlledLegs;

	void UpdateWalkingBobCycle();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	float WalkingBobCycle;

	EAnimationState AnimationState;
	void SetState(EAnimationState);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	TArray<FVector> LegPositions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	TArray<FRotator> LegRotations;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	UCurveVector* BackLegMoveCurve;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	UCurveFloat* BackLegRotateCurve;
};