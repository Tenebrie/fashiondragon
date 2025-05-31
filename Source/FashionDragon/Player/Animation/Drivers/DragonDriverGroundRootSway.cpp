#include "DragonDriverGroundRootSway.h"

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegDriver.h"

void FDragonDriverGroundRootSway::Tick(const float DeltaTime)
{
	BlendAlpha = FMath::FInterpTo(BlendAlpha, TargetBlendAlpha, DeltaTime, 5.0f);

	if (LeadingLeg)
	{
		const auto LeadingLegOffset = std::min(1.0, LeadingLeg->GetLeg()->Position.Size() / 750.0f);
		const auto SideOffsetSign = LeadingLeg->GetLeg()->GetIdx() == 0 ? 1.0f : -1.0f;
	
		const auto ForwardOffset = FMath::Max(400.0f - LeadingLeg->GetLeg()->Position.Y, 0.0f) / 15.0f;
		const auto CurrentVerticalOffset = LeadingLegOffset * -VerticalAmplitude;

		const auto LastPos = DesiredPosition - FVector(0, 0, VerticalOffset);
		DesiredPosition = FMath::VInterpTo(LastPos, FVector(SideOffsetSign * HorizontalAmplitude, ForwardOffset, CurrentVerticalOffset), DeltaTime, 5.5f);
		DesiredPosition += FVector(0.0f, 0.0f, VerticalOffset);
	}

	// Lerp current value to target value
	auto TargetLean = -20.0f;

	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	const auto HorizontalMovementSpeed = OwningActor->GetVelocity().Size2D();
	TargetLean += FMath::Clamp(HorizontalMovementSpeed * 0.007f, 0.0f, 20.0f);

	DesiredForce = 25.0f;
	DesiredRotation = FRotator(0, 0, TargetLean);
	// DesiredRotation = FRotator(0.0f, 0.0f, 0.0f);
}
