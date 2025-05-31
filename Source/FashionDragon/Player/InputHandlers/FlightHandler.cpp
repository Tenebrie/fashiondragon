#include "FlightHandler.h"

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

void UFlightHandler::BeginPlay()
{
	Super::BeginPlay();
	LeftWing = GetAnimInstance()->LeftWing.GetPostProcessBone();
	RightWing = GetAnimInstance()->RightWing.GetPostProcessBone();
}

void UFlightHandler::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const auto Character = GetCharacter();
	if (!IsInFlight || !Character || !Character->GetMovementComponent()) { return; }

	const auto MovementComponent = GetCharacter()->GetCharacterMovement();

	const auto FlapHappened = (LeftWing->Flap - LastFrameLeftFlap) * LeftWing->Openness + (RightWing->Flap - LastFrameRightFlap) * RightWing->Openness;

	const FVector LiftVector = FVector(0.0f, 0.0f, FlapHappened * 900.0f);

	MovementComponent->AddImpulse(LiftVector, true);

	LastFrameLeftFlap = LeftWing->Flap;
	LastFrameRightFlap = RightWing->Flap;
	LastFrameLeftOpenness = LeftWing->Openness;
	LastFrameRightOpenness = RightWing->Openness;

	// MovementComponent->Velocity = FVector::ZeroVector;
}

void UFlightHandler::StartFlight()
{
	IsInFlight = true;
}

