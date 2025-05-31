#include "DragonDriverTurnToMovement.h"

#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

void FDragonDriverTurnToMovement::Tick(const float DeltaTime)
{
	FProceduralBoneDriver::Tick(DeltaTime);

	// AnimInstance->GetCharacter()->GetLast
}

FPoseEffector FDragonDriverTurnToMovement::ToEffector(const FPoseEffector& BaseEffector,
	const FPoseEffectorContext& Context)
{
	return FProceduralBoneDriver::ToEffector(BaseEffector, Context);
}
