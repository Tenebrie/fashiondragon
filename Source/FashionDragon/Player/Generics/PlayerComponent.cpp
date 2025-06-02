#include "PlayerComponent.h"

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

AMainCharacter* UPlayerComponent::GetCharacter() const
{
	return Cast<AMainCharacter>(GetOwner());
}

UDragonAnimInstance* UPlayerComponent::GetAnimInstance() const
{
	return Cast<UDragonAnimInstance>(GetCharacter()->DragonMesh->GetAnimInstance());
}

UCharacterMovementComponent* UPlayerComponent::GetCharacterMovement() const
{
	const auto Character = GetCharacter();
	if (!Character) { return nullptr; }
	return Character->GetCharacterMovement();
}
