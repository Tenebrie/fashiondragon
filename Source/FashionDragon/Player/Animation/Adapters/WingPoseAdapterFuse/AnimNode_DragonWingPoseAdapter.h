#pragma once
#include "Fuse_DragonWingPoseAdapter.h"
#include "FashionDragon/Player/Animation/Structs/PoseWingEffector.h"
#include "AnimNode_DragonWingPoseAdapter.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FAnimNode_DragonWingPoseAdapter : public FAnimNode_Base
{
	GENERATED_BODY()

	// Input pose
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
	FPoseLink InputPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (PinShownByDefault))
	FPoseWingEffector LeftWingEffector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (PinShownByDefault))
	FPoseWingEffector RightWingEffector;

	// --- Internals ---
	virtual void Evaluate_AnyThread(FPoseContext& Output) override
	{
		// Evaluate the input pose first
		InputPose.Evaluate(Output);

		FFuse_DragonWingPoseAdapter::Evaluate(Output, LeftWingEffector, RightWingEffector);
	}

	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override
	{
		InputPose.Initialize(Context);
	}

	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override
	{
		InputPose.CacheBones(Context);
	}

	virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override
	{
		InputPose.Update(Context);
	}

	virtual void GatherDebugData(FNodeDebugData& DebugData) override
	{
		InputPose.GatherDebugData(DebugData);
	}
};
