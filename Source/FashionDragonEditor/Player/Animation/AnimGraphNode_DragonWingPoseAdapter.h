#pragma once
#include "Animation/AnimNodeBase.h"
#include "Editor/AnimGraph/Public/AnimGraphNode_Base.h"
#include "FashionDragon/Player/Animation/Adapters/WingPose/AnimNode_DragonWingPoseAdapter.h"
#include "AnimGraphNode_DragonWingPoseAdapter.generated.h"

UCLASS()
class UAnimGraphNode_DragonWingPoseAdapter : public UAnimGraphNode_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Settings)
	FAnimNode_DragonWingPoseAdapter Node;

	// Node title
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override
	{
		return FText::FromString(TEXT("Wing Pose Adapter"));
	}
};
