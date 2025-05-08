#include "DragonAnimInstance.h"

void UDragonAnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UDragonAnimInstance::Advance(const float Value, const float DeltaTime)
{
	/**
	 * 0: FrontLeftPaw
	 * 1: FrontRightPaw
	 * 2: BackLeftPaw
	 * 3: BackRightPaw
	 */

	
}

void UDragonAnimInstance::AdvanceLegacy(const float Value, const float DeltaTime)
{
	/**
	 * 0: FrontLeftPaw
	 * 1: FrontRightPaw
	 * 2: BackLeftPaw
	 * 3: BackRightPaw
	 */
	
	const auto MovingForward = Value > 0.0f;
	const auto MovingBackwards = Value < 0.0f;

	// Launch off
	if (MovementState == 0 && Value != 0.0f)
	{
		MovementState = 1;
		RightBackPawMoveState = 2.0f;
	}

	const float WalkAnimationAlphaDelta = (GetOwningActor()->GetVelocity().Size() / 100.f) * DeltaTime;
	WalkAnimationAlpha += WalkAnimationAlphaDelta;
	WalkAnimationAlpha = FMath::Clamp(WalkAnimationAlpha, 0.0f, 1.0f);
	if (WalkAnimationAlpha == 0.0f)
	{
		MovementState = 0;
		LeftBackPawMoveState = 0.0f;
		RightBackPawMoveState = 0.0f;
	}

	// BackLeftPaw
	this->LeftBackPawMoveState += Value;
	if (MovingForward && this->LeftBackPawMoveState > 5.0f && !bIsLeftBackPawLocked)
	{
		this->LeftBackPawMoveState = 1.0f;
		LockPawTarget(2);
	}
	else if (MovingForward && LeftBackPawMoveState > 4.5f && bIsLeftBackPawLocked)
	{
		UnlockPawTarget(2);
	}
	
	if (MovingBackwards && this->LeftBackPawMoveState < 1.0f)
	{
		this->LeftBackPawMoveState = 5.0f;
	}
	UpdatePawTarget(2);

	// BackRightPaw
	this->RightBackPawMoveState += Value;
	if (MovingForward && this->RightBackPawMoveState > 5.0f && !bIsRightBackPawLocked)
	{
		this->RightBackPawMoveState = 1.0f;
		LockPawTarget(3);
	}
	else if (MovingForward && RightBackPawMoveState > 4.5f && bIsRightBackPawLocked)
	{
		UnlockPawTarget(3);
	}
	if (MovingBackwards && this->RightBackPawMoveState < 1.0f)
	{
		this->RightBackPawMoveState = 5.0f;
	}
	UpdatePawTarget(3);
}

void UDragonAnimInstance::UpdatePawTarget(const int Index)
{
	auto MoveState = 0.0f;
	if (Index == 2)
		MoveState = LeftBackPawMoveState;
	else if (Index == 3)
		MoveState = RightBackPawMoveState;
	
	const auto Curve = BackLegMoveCurve;
	const auto StepOffset = BackPawOffsetPosition;
	
	const auto CurveValue = Curve->GetVectorValue(MoveState);
	const auto TargetLocal = FVector(
		CurveValue.X * StepOffset.X,
		CurveValue.Y * StepOffset.Y,
		CurveValue.Z * StepOffset.Z
	);
	const auto TargetGlobal = GetSkelMeshComponent()->GetComponentTransform().TransformPosition(TargetLocal);

	if (Index == 2)
	{
		this->LeftBackPawPosition = TargetGlobal;
		if (!bIsLeftBackPawLocked)
			this->LeftBackPawLockedPosition = TargetGlobal;
	}
	else if (Index == 3)
	{
		this->RightBackPawPosition = TargetGlobal;
		if (!bIsRightBackPawLocked)
			this->RightBackPawLockedPosition = TargetGlobal;
	}
}

void UDragonAnimInstance::LockPawTarget(const int Index)
{
	if (Index == 2)
		this->bIsLeftBackPawLocked = true;
	else if (Index == 3)
		this->bIsRightBackPawLocked = true;
}

void UDragonAnimInstance::UnlockPawTarget(const int Index)
{
	if (Index == 2)
		this->bIsLeftBackPawLocked = false;
	else if (Index == 3)
		this->bIsRightBackPawLocked = false;
}
