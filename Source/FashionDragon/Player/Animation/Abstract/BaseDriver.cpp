#include "BaseDriver.h"

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

float FBaseDriver::GetRawInputRotation() const
{
	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	if (!OwningActor) { return 0; }
	
	auto VelocityVector = OwningActor->GetCharacterMovement()->GetLastUpdateVelocity();
	VelocityVector.Normalize();
	if (VelocityVector.IsNearlyZero()) { return 0; }
	
	// Get normalized input vector in local space
	const auto InputVector = VelocityVector.GetSafeNormal2D();

	// Calculate the angle between input vector and forward vector directly
	const float AngleRadians = FMath::Atan2(InputVector.Y, InputVector.X);

	return AngleRadians;
	// return FMath::RadiansToDegrees(AngleRadians);
}

float FBaseDriver::GetInputRotation() const
{
	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	if (!OwningActor) { return 0; }
	
	auto VelocityVector = OwningActor->GetCharacterMovement()->GetLastUpdateVelocity();
	VelocityVector.Normalize();
	if (VelocityVector.IsNearlyZero()) { return 0; }
	
	// Get normalized input vector in local space
	const auto InputVector = OwningActor->GetActorRotation().UnrotateVector(VelocityVector).GetSafeNormal2D();

	// Calculate the angle between input vector and forward vector directly
	const float AngleRadians = FMath::Atan2(InputVector.Y, InputVector.X);

	return AngleRadians;
	// return FMath::RadiansToDegrees(AngleRadians);
}

FVector FBaseDriver::RotateVectorToInputRotation(const FVector& VectorToRotate, const bool Inverted) const
{
	const auto OwningActor = Cast<AMainCharacter>(AnimInstance->GetOwningActor());
	if (!OwningActor) { return VectorToRotate; }
	
	auto VelocityVector = OwningActor->GetCharacterMovement()->GetLastUpdateVelocity();
	VelocityVector.Normalize();
	if (VelocityVector.IsNearlyZero()) { return VectorToRotate; }
	
	// Get normalized input vector in local space
	// const auto ActorRotation = OwningActor->GetActorRotation();
	const auto ActorRotation = AnimInstance->GetSkelMeshComponent()->GetAttachParent()->GetComponentRotation() + FRotator(0.f, 90.f, 0.f);
	const auto InputVector = ActorRotation.UnrotateVector(VelocityVector).GetSafeNormal2D();
	// const auto InputVector = VelocityVector;

	// Calculate the angle between input vector and forward vector directly
	float AngleRadians = FMath::Atan2(InputVector.Y, InputVector.X);
	if (Inverted)
	{
		AngleRadians = -AngleRadians;
	}

	// Create rotation matrix around Z axis using the angle
	const FRotator Rotation(0, FMath::RadiansToDegrees(AngleRadians), 0);
	const FRotationMatrix RotMatrix(Rotation);

	// Apply rotation to target position
	return RotMatrix.TransformPosition(VectorToRotate);
}