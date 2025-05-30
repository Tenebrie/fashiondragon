#include "ProceduralPose.h"

#include "FashionDragon/Player/Animation/Structs/PoseWingEffector.h"

FProceduralPose::FProceduralPose(UDragonAnimInstance* AnimInstance):
	AnimInstance(AnimInstance)
{}

void FProceduralPose::NativeBeginPlay()
{
	for (const auto &RootDriver : RootDrivers)
		RootDriver->NativeBeginPlay();
	for (const auto &TailDriver : TailDrivers)
		TailDriver->NativeBeginPlay();
	for (const auto &LegDriver : LegDrivers)
		LegDriver->NativeBeginPlay();
	for (const auto &WingDriver : WingDrivers)
		WingDriver->NativeBeginPlay();
}

void FProceduralPose::Tick(const float DeltaTime)
{
	for (const auto &RootDriver : RootDrivers)
		RootDriver->Tick(DeltaTime);
	for (const auto &TailDriver : TailDrivers)
		TailDriver->Tick(DeltaTime);
	for (const auto &LegDriver : LegDrivers)
		LegDriver->Tick(DeltaTime);
	for (const auto &WingDriver : WingDrivers)
		WingDriver->Tick(DeltaTime);
}

FPoseEffector FProceduralPose::ToRootEffector(const FPoseEffector& BaseEffector, const FControlledBone* Body, const float DeltaTime) const
{
	for (const auto& RootDriver : RootDrivers)
	{
		if (RootDriver && RootDriver->GetBone() == Body)
		{
			const auto Context = FPoseEffectorContext(DeltaTime, RootDriver->GetBlendAlpha());
			return RootDriver->ToEffector(BaseEffector, Context);
		}
	}
	return BaseEffector;
}

FPoseEffector FProceduralPose::ToTailEffector(const FPoseEffector& BaseEffector, const FControlledBone* Hips, const float DeltaTime) const
{
	for (const auto& TailDriver : TailDrivers)
	{
		if (TailDriver && TailDriver->GetBone() == Hips)
		{
			const auto Context = FPoseEffectorContext(DeltaTime, TailDriver->GetBlendAlpha());
			return TailDriver->ToEffector(BaseEffector, Context);
		}
	}
	return BaseEffector;
}

FPoseEffector FProceduralPose::ToLegEffector(const FPoseEffector& BaseEffector, const FControlledLeg* Leg, const float DeltaTime) const
{
	for (const auto& LegDriver : LegDrivers)
	{
		if (LegDriver && LegDriver->GetLeg() == Leg)
		{
			const auto Context = FPoseEffectorContext(DeltaTime, LegDriver->GetBlendAlpha());
			return LegDriver->ToEffector(BaseEffector, Context);
		}
	}
	return BaseEffector;
}

FPoseEffector FProceduralPose::ToPostProcessLegEffector(const FPoseEffector& BaseEffector, const FControlledLeg* Leg, const float DeltaTime) const
{
	for (const auto& LegDriver : LegDrivers)
	{
		if (LegDriver && LegDriver->GetLeg() == Leg)
		{
			const auto Context = FPoseEffectorContext(DeltaTime, LegDriver->GetBlendAlpha());
			return LegDriver->ToPostProcessEffector(BaseEffector, Context);
		}
	}
	return BaseEffector;
}

FPoseWingEffector FProceduralPose::ToWingEffector(const FPoseWingEffector& BaseEffector, const FControlledWing* Wing, const float DeltaTime) const
{
	for (const auto& WingDriver : WingDrivers)
	{
		if (WingDriver && WingDriver->GetWing() == Wing)
		{
			const auto Context = FPoseEffectorContext(DeltaTime, WingDriver->GetBlendAlpha());
			return WingDriver->ToEffector(BaseEffector, Context);
		}
	}
	return BaseEffector;
}

void FProceduralPose::AddBlendAlpha(const float Delta)
{
	for (FProceduralBoneDriver* RootDriver : RootDrivers)
		RootDriver->SetBlendAlpha(FMath::Clamp(RootDriver->GetBlendAlpha() + Delta, 0, 1));
	for (FProceduralBoneDriver* TailDriver : TailDrivers)
		TailDriver->SetBlendAlpha(FMath::Clamp(TailDriver->GetBlendAlpha() + Delta, 0, 1));
	for (FProceduralLegDriver* LegDriver : LegDrivers)
		LegDriver->SetBlendAlpha(FMath::Clamp(LegDriver->GetBlendAlpha() + Delta, 0, 1));
	for (FProceduralWingDriver* WingDriver : WingDrivers)
		WingDriver->SetBlendAlpha(FMath::Clamp(WingDriver->GetBlendAlpha() + Delta, 0, 1));
}

void FProceduralPose::SetBlendAlpha(const float BlendAlpha)
{
	for (FProceduralBoneDriver* RootDriver : RootDrivers)
		RootDriver->SetBlendAlpha(BlendAlpha);
	for (FProceduralBoneDriver* TailDriver : TailDrivers)
		TailDriver->SetBlendAlpha(BlendAlpha);
	for (FProceduralLegDriver* LegDriver : LegDrivers)
		LegDriver->SetBlendAlpha(BlendAlpha);
	for (FProceduralWingDriver* WingDriver : WingDrivers)
		WingDriver->SetBlendAlpha(BlendAlpha);
}
