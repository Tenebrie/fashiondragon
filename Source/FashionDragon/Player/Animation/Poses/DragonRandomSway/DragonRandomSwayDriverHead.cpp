#include "DragonRandomSwayDriverHead.h"

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

void FDragonRandomSwayDriverHead::Tick(const float DeltaTime)
{
	FProceduralBoneDriver::Tick(DeltaTime);

	TransitionPosition += DeltaTime;
	if (TransitionPosition >= TransitionDuration)
		TransitionPosition = TransitionDuration;

	if (Cooldown > 0)
	{
		Cooldown -= DeltaTime;
		return;
	}

	TransitionPosition = 0.0f;
	TransitionDuration = FMath::RandRange(0.7f, 1.5f);
	TransitioningFrom = Bone->Rotation;

	if (HeadLookState == EHeadLookState::LookingAtTarget
		|| (HeadLookState == EHeadLookState::LookingAround && LookingAroundStatesLeft <= 0))
	{
		HeadLookState = EHeadLookState::Relaxed;
		DesiredRotation = FRotator::ZeroRotator;
		Cooldown = FMath::RandRange(7.5f, 15.0f);
		return;
	}

	if (HeadLookState == EHeadLookState::Relaxed)
	{
		if (FMath::RandBool())
		{
			HeadLookState = EHeadLookState::LookingAround;
			LookingAroundStatesLeft = FMath::RandRange(2, 4);
		}
		else
		{
			HeadLookState = EHeadLookState::LookingAtTarget;
			constexpr float MaxPitch = 0.0f;
			constexpr float MaxYaw = 20.0f;
			constexpr float MaxRoll = 20.0f;

			DesiredRotation = FRotator(
				FMath::RandRange(-MaxPitch, MaxPitch),
				FMath::RandRange(-MaxYaw, MaxYaw),
				FMath::RandRange(-10.0f, MaxRoll)
			);
			Cooldown = FMath::RandRange(2.0f, 4.0f);
			return;
		}
	}

	if (HeadLookState == EHeadLookState::LookingAround && LookingAroundStatesLeft > 0)
	{
		LookingAroundStatesLeft -= 1;
		constexpr float MaxPitch = 0.0f;
		constexpr float MaxYaw = 20.0f;
		constexpr float MaxRoll = 20.0f;

		DesiredRotation = FRotator(
			FMath::RandRange(-MaxPitch, MaxPitch),
			FMath::RandRange(-MaxYaw, MaxYaw),
			FMath::RandRange(-10.0f, MaxRoll)
		);
		Cooldown = FMath::RandRange(1.5f, 2.5f);
	}
}

FPoseEffector FDragonRandomSwayDriverHead::ToEffector(const FPoseEffector& BaseEffector, const FPoseEffectorContext& Context)
{
	const float EasedAlpha = FMath::InterpEaseInOut(0.0f, 1.0f, TransitionPosition / TransitionDuration, 4.f);

	FRotator Result;
	Result.Pitch = FMath::Lerp(TransitioningFrom.Pitch, DesiredRotation.Pitch, EasedAlpha);
	Result.Yaw = FMath::Lerp(TransitioningFrom.Yaw, DesiredRotation.Yaw, EasedAlpha);
	Result.Roll = FMath::Lerp(TransitioningFrom.Roll, DesiredRotation.Roll, EasedAlpha);

	return BaseEffector.SetRotation(Result);
}
