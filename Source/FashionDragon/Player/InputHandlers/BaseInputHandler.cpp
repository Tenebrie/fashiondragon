#include "BaseInputHandler.h"

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

AMainCharacter* UBaseInputHandler::GetCharacter() const
{
	return Cast<AMainCharacter>(GetOwner());
}

UDragonAnimInstance* UBaseInputHandler::GetAnimInstance() const
{
	return Cast<UDragonAnimInstance>(GetCharacter()->DragonMesh->GetAnimInstance());
}

UCharacterMovementComponent* UBaseInputHandler::GetCharacterMovement() const
{
	const auto Character = GetCharacter();
	if (!Character) { return nullptr; }
	return Character->GetCharacterMovement();
}
