#include "DragonWingPoseAdapterApplicator.h"

#include "Animation/AnimInstanceProxy.h"
#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"

void FDragonWingPoseAdapterApplicator::Evaluate(FPoseContext& Output, const FPoseWingEffector& LeftWingEffector, const FPoseWingEffector& RightWingEffector)
{
	const auto LeftWingBones = { 
		"Wing_001_L", "Wing_002_L", "Wing_010_L", "Wing_011_L", "Wing_020_L",
		"Wing_021_L", "Wing_030_L", "Wing_031_L", "Wing_040_L", "Wing_041_L"
	};
	const auto RightWingBones = { 
		"Wing_001_R", "Wing_002_R", "Wing_010_R", "Wing_011_R", "Wing_020_R",
		"Wing_021_R", "Wing_030_R", "Wing_031_R", "Wing_040_R", "Wing_041_R"
	};
	for (const FName& BoneName : LeftWingBones)
	{
		ProcessBoneAsDriven(Output, BoneName, LeftWingEffector);
	}
	for (const FName& BoneName : RightWingBones)
	{
		ProcessBoneAsDriven(Output, BoneName, RightWingEffector);
	}
	
	ProcessBoneAsHelper(Output, "Wing_Helper_015_L", {"Wing_010_L", "Wing_020_L"});
	ProcessBoneAsHelper(Output, "Wing_Helper_016_L", {"Wing_011_L", "Wing_021_L"});
	ProcessBoneAsHelper(Output, "Wing_Helper_025_L", {"Wing_020_L", "Wing_030_L"});
	ProcessBoneAsHelper(Output, "Wing_Helper_026_L", {"Wing_021_L", "Wing_031_L"});
	ProcessBoneAsHelper(Output, "Wing_Helper_035_L", {"Wing_030_L", "Wing_040_L"});
	ProcessBoneAsHelper(Output, "Wing_Helper_036_L", {"Wing_031_L", "Wing_041_L"});
	ProcessBoneAsHelper(Output, "Wing_Helper_006_L", {"Wing_001_L", "Wing_002_L", "Wing_040_L"});
	ProcessBoneAsHelper(Output, "Wing_Helper_005_L", {"Wing_001_L", "Wing_Helper_006_L"});
	// Needs special handling
	ProcessBoneAsHelper(Output, "Wing_Helper_007_L", {"Wing_002_L", "Wing_040_L"});

	ProcessBoneAsHelper(Output, "Wing_Helper_015_R", {"Wing_010_R", "Wing_020_R"});
	ProcessBoneAsHelper(Output, "Wing_Helper_016_R", {"Wing_011_R", "Wing_021_R"});
	ProcessBoneAsHelper(Output, "Wing_Helper_025_R", {"Wing_020_R", "Wing_030_R"});
	ProcessBoneAsHelper(Output, "Wing_Helper_026_R", {"Wing_021_R", "Wing_031_R"});
	ProcessBoneAsHelper(Output, "Wing_Helper_035_R", {"Wing_030_R", "Wing_040_R"});
	ProcessBoneAsHelper(Output, "Wing_Helper_036_R", {"Wing_031_R", "Wing_041_R"});
	ProcessBoneAsHelper(Output, "Wing_Helper_006_R", {"Wing_001_R", "Wing_002_R", "Wing_040_R"});
	ProcessBoneAsHelper(Output, "Wing_Helper_005_R", {"Wing_001_R", "Wing_Helper_006_R"});
	// Needs special handling
	ProcessBoneAsHelper(Output, "Wing_Helper_007_R", {"Wing_002_R", "Wing_040_R"});
}

void FDragonWingPoseAdapterApplicator::ProcessBoneAsDriven(FPoseContext& Output, const FName& BoneName,
	const FPoseWingEffector& Effector)
{
	const FBoneContainer& BoneContainer = Output.AnimInstanceProxy->GetRequiredBones();
	const FCompactPoseBoneIndex BoneIndex(BoneContainer.GetPoseBoneIndexForBoneName(BoneName));

	if (BoneIndex == INDEX_NONE)
		return;

	FTransform& OutTransform = Output.Pose[BoneIndex];

	const FTransform Offset = FBaseDragonWingPoseAdapter::ProcessBone(BoneName, Effector);
	
	OutTransform = Offset * OutTransform;
}

void FDragonWingPoseAdapterApplicator::ProcessBoneAsHelper(
        FPoseContext& Output,
        const FString& BoneName,
        const TArray<FString>& Parents)
{
    const FBoneContainer& BoneContainer = Output.AnimInstanceProxy->GetRequiredBones();

    const FCompactPoseBoneIndex BoneIndex(
        BoneContainer.GetPoseBoneIndexForBoneName(FName(BoneName)));

    if (BoneIndex == INDEX_NONE)
        return;

    /* ---------------------------------------------------------
     * 1)  Grab the helper-bone’s current pose AND its ref-pose
     * --------------------------------------------------------*/
    FTransform& BoneCurrent = Output.Pose[BoneIndex];
    const FTransform BoneRest = BoneContainer.GetRefPoseTransform(BoneIndex);

    /* ---------------------------------------------------------
     * 2)  Collect both current- and rest-transforms for parents
     * --------------------------------------------------------*/
    TArray<FTransform> ParentsCurrent;
    TArray<FTransform> ParentsRest;

    for (const FString& ParentName : Parents)
    {
        const FCompactPoseBoneIndex ParentIndex(
            BoneContainer.GetPoseBoneIndexForBoneName(FName(ParentName)));

        if (ParentIndex != INDEX_NONE)
        {
            ParentsCurrent.Add(Output.Pose[ParentIndex]);               // live pose
            ParentsRest   .Add(BoneContainer.GetRefPoseTransform(ParentIndex));
        }
    }

    ProcessHelperBone(
        BoneCurrent,        // out-param: will be overwritten
        BoneRest,              // helper’s own rest transform
        ParentsCurrent,        // live parents
        ParentsRest            // rest-pose parents
    );
}

void FDragonWingPoseAdapterApplicator::ProcessHelperBone(FTransform& Bone, const FTransform& RestOffset,
	const TArray<FTransform>& Parents, const TArray<FTransform>& ParentsRest)
{
	const int32 NumParents = Parents.Num();
	if (NumParents == 0 || NumParents != ParentsRest.Num())
	{
		Bone = RestOffset;          // nothing to do or bad data
		return;
	}

	// ----- accumulate the offsets ------------------------------------------------
	FVector AccumLoc   = FVector::ZeroVector;
	FVector AccumScale = FVector::ZeroVector;
	FQuat   AccumQuat  = FQuat::Identity;

	for (int32 i = 0; i < NumParents; ++i)
	{
		const FTransform& Cur  = Parents[i];
		const FTransform& Rest = ParentsRest[i];

		// how much THIS parent moved away from rest pose
		const FTransform Delta = Cur * Rest.Inverse();

		AccumLoc   += Delta.GetTranslation();
		AccumScale += Delta.GetScale3D();

		// incremental quaternion average (slerp keeps the result normalised)
		const float InvCount = 1.f / static_cast<float>(i + 1);
		AccumQuat   = FQuat::Slerp(AccumQuat, Delta.GetRotation(), InvCount);
	}

	const float InvNum = 1.f / static_cast<float>(NumParents);

	const FVector AvgLoc   = AccumLoc * InvNum;
	const FVector AvgScale = AccumScale * InvNum;
	AccumQuat.Normalize();                       // make sure it’s unit length

	// ----- build the averaged delta and apply it to the helper rest pose --------
	const FTransform AvgDelta(AccumQuat, AvgLoc, AvgScale);

	Bone = AvgDelta * RestOffset;             // first apply delta, THEN rest
}
