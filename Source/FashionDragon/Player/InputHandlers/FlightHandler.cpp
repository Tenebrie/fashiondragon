#include "FlightHandler.h"

#include "RotationInputHandler.h"
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
}

void UFlightHandler::StartFlight()
{
	IsInFlight = true;
}

void UFlightHandler::CancelFlight()
{
	IsInFlight = false;
}

void UFlightHandler::HandleFlapForwardInput(const FInputActionValue& Value)
{
	bIsFlapping = Value.Get<bool>();
}

void UFlightHandler::HandleFoldWingsInput(const FInputActionValue& Value)
{
	bIsFoldingWings = Value.Get<bool>();
	if (bIsFoldingWings)
	{
		WingAreaFactor = 0.0f;
	}
	else
	{
		WingAreaFactor = 1.0f;
	}
}

