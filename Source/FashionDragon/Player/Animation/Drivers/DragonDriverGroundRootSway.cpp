#include "DragonDriverGroundRootSway.h"

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegDriver.h"
#include "GameFramework/CharacterMovementComponent.h"

void FDragonDriverGroundRootSway::Tick(const float DeltaTime)
{
	FProceduralBoneDriver::Tick(DeltaTime);

	DesiredForce = 25.0f;
	
	// Up/down bobbing when stepping
	if (LeadingLeg && LeadingLeg->GetLeg())
	{
		const auto LeadingLegOffset = std::min(1.0, LeadingLeg->GetLeg()->Position.Size() / 750.0f);
		const auto SideOffsetSign = LeadingLeg->GetLeg()->GetIdx() == 0 ? 1.0f : -1.0f;
	
		const auto ForwardOffset = FMath::Max(400.0f - LeadingLeg->GetLeg()->Position.Y, 0.0f) / 15.0f;
		const auto CurrentVerticalOffset = LeadingLegOffset * -VerticalAmplitude;

		const auto LastPos = DesiredPosition - FVector(0, 0, VerticalOffset);
		DesiredPosition = FMath::VInterpTo(LastPos, FVector(ForwardOffset, SideOffsetSign * HorizontalAmplitude, CurrentVerticalOffset), DeltaTime, 5.5f);
		DesiredPosition += FVector(0.0f, 0.0f, VerticalOffset);
	}

	// Forward leaning based on horizontal movement speed
	{
		auto TargetLean = -20.0f;

		const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
		const auto HorizontalMovementSpeed = OwningActor->GetVelocity().Size2D();
		TargetLean += FMath::Clamp(HorizontalMovementSpeed * 0.007f, 0.0f, 20.0f);
	
		DesiredRotation = FRotator(-TargetLean, 0, 0);
	}

	// Lean into ground gradient
	{
		const auto CurLeftPos = LeftLeg->GetPlantedWorldPosition(FVector::ZeroVector, FRotator::ZeroRotator, 300.0f);
		const auto CurRightPos = RightLeg->GetPlantedWorldPosition(FVector::ZeroVector, FRotator::ZeroRotator, 300.0f);
		const auto ProbeLeftPos = LeftLeg->GetPlantedWorldPosition(FVector(1000, 0, 0), FRotator::ZeroRotator, 300.0f);
		const auto ProbeRightPos = RightLeg->GetPlantedWorldPosition(FVector(1000, 0, 0), FRotator::ZeroRotator, 300.0f);

		const float CurLeftDepth = CurLeftPos.GroundHit ? CurLeftPos.DeltaPosition.Z : -300.0f;
		const float CurRightDepth = CurRightPos.GroundHit ? CurRightPos.DeltaPosition.Z : -300.0f;
		const float ProbeLeftDepth = ProbeLeftPos.GroundHit ? ProbeLeftPos.DeltaPosition.Z : -300.0f;
		const float ProbeRightDepth = ProbeRightPos.GroundHit ? ProbeRightPos.DeltaPosition.Z : -300.0f;

		const auto LeftGradient = ProbeLeftDepth - CurLeftDepth;
		const auto RightGradient = ProbeRightDepth - CurRightDepth;
		const float ForwardGradient = (LeftGradient + RightGradient) / 2.0f;
		const float SideGradient = CurLeftDepth - CurRightDepth;

		CurrentGradient = FMath::FInterpTo(CurrentGradient, ForwardGradient, DeltaTime, 2.0f);

		DesiredRotation.Roll += SideGradient / 30.0f;
		DesiredRotation.Pitch += CurrentGradient / 20.0f;
	}
}
