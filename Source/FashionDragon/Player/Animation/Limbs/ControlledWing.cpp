#include "ControlledWing.h"

FName FControlledWing::GetBoneName(const int BoneIdx) const
{
	const auto Suffix = Idx == 0 ? "_L" : "_R";

	FString Name;
	switch (BoneIdx)
	{
	case 0:
		Name = FString("Wing_001") + Suffix;
		break;
	case 1:
		Name = FString("Wing_002") + Suffix;
		break;
	case 2:
		Name = FString("Wing_Finger_001") + Suffix;
		break;
	case 3:
		Name = FString("Wing_Finger_002") + Suffix;
		break;
	case 4:
		Name = FString("Wing_Finger_003") + Suffix;
		break;
	case 5:
		Name = FString("Wing_Finger_004") + Suffix;
		break;
	case 6:
		Name = FString("Wing_Finger_005") + Suffix;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Invalid bone index %d for wing %d"), BoneIdx, Idx);
		Name = FString("Wing_001") + Suffix;
	}
	
	return FName(Name);
}
