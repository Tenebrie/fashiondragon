#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Curves/CurveVector.h"
#include "DragonAnimInstance.generated.h"

UCLASS()
class FASHIONDRAGON_API UDragonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:
	class FControlledLeg;

	std::vector<FControlledLeg*> ControlledLegs = std::vector<FControlledLeg*>();

public:
	UDragonAnimInstance();
	
	int MovementState = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="IK")
	float WalkAnimationAlpha = 0.0f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="IK")
	FVector3f BackPawOffsetPosition = FVector3f(0.0f, 150.0f, 120.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="IK")
	FRotator BackPawOffsetRotation = FRotator(0.0f, 0.0f, 30.0f);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="IK")
	float LeftBackPawMoveState = 0.0f; // 0: Idle, 1: Start of cycle, 5: End of cycle (return to 1)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="IK")
	float RightBackPawMoveState = 2.0f; // 0: Idle, 1: Start of cycle, 5: End of cycle (return to 1)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="LockPositions")
	bool bIsLeftBackPawLocked = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="LockPositions")
	FVector LeftBackPawPosition;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="LockPositions")
	FVector LeftBackPawLockedPosition;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="LockPositions")
	bool bIsRightBackPawLocked = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="LockPositions")
	FVector RightBackPawPosition;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="LockPositions")
	FVector RightBackPawLockedPosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	UCurveVector* BackLegMoveCurve;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Assets")
	UCurveFloat* BackLegRotateCurve;

	void Advance(float Value, float DeltaTime);
	void AdvanceLegacy(float Value, float DeltaTime);

	void LockPawTarget(int Index);
	void UnlockPawTarget(int Index);
	void UpdatePawTarget(int Index);

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};