#include "AnimationDebugReporter.h"

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"
#include "FashionDragon/Utils/Utils.h"

void UAnimationDebugReporter::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CollectInfo();
}

void UAnimationDebugReporter::CollectInfo()
{
	const auto Player = Cast<AMainCharacter>(GetOwner());
	if (!Player || !Player->DragonMesh) { return; }
	const auto AnimInstance = Cast<UDragonAnimInstance>(Player->DragonMesh->GetAnimInstance());
	if (!AnimInstance) { return; }

	PoseDebugInfo = TArray<FPoseDebugInfo>();

	for (const auto Pose : AnimInstance->StateMachine->PoseDrivers)
	{
		FPoseDebugInfo PoseInfo;
		PoseInfo.Name = Pose->GetClassName();
		for (FProceduralBoneDriver* Driver : Pose->ListBoneDrivers())
		{
			FDriverDebugInfo Data = Driver->GetDebugReporter()->MakeDebugInfo();
			Data.Name = Driver->GetClassName();
			Data.GroupName = Driver->GetBone()->GroupName;
			Data.LayerName = FUtils::EnumToString(Driver->GetBone()->Layer);
			Data.BlendAlpha = Driver->GetBlendAlpha();
			PoseInfo.Drivers.Add(Data);
		}

		for (FProceduralLegDriver* Driver : Pose->ListLegDrivers())
		{
			FDriverDebugInfo Data = Driver->GetDebugReporter()->MakeDebugInfo();
			Data.Name = Driver->GetClassName();
			Data.GroupName = Driver->GetLeg()->GroupName;
			Data.LayerName = FUtils::EnumToString(Driver->GetLeg()->Layer);
			Data.BlendAlpha = Driver->GetBlendAlpha();
			PoseInfo.Drivers.Add(Data);
		}

		PoseDebugInfo.Add(PoseInfo);
	}
}
