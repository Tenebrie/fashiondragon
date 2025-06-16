#include "BaseDragonWingPoseAdapter.h"

#include "FashionDragon/DebugTools/QuickDebug.h"

enum class EWingMovementAxis
{
	Closeness,
	FlapAngle,
	TiltAngle
};

FTransform FBaseDragonWingPoseAdapter::ProcessBone(const FName& FullBoneName, const FPoseWingEffector& Effector)
{
	TMap<EWingMovementAxis, TMap<FString, FRotator>> TransformData =
	{
		{
			EWingMovementAxis::Closeness,{
				{ "Wing_001", FRotator(60.0, -20, -20) },
				{ "Wing_002", FRotator(-115.0, 0, 0) },
				{ "Wing_010", FRotator(120.0, 0, 0) },
				{ "Wing_020", FRotator(100.0, 0, 0) },
				{ "Wing_030", FRotator(85.0, 0, 0) },
				{ "Wing_040", FRotator(60.0, 0, 0) },
			}
		},
		{
			EWingMovementAxis::FlapAngle,{
								 
			}
		},
		{
			EWingMovementAxis::TiltAngle,{
								 
			}
		}
	};
	
	FTransform OutTransform = FTransform();
	const FString Name = FullBoneName.ToString().LeftChop(2);

	const float Closeness = 1.0f - Effector.Openness;
	const float FlapAngle = Effector.FlapAngle;
	const float TiltAngle = Effector.TiltAngle;

	if (TransformData[EWingMovementAxis::Closeness].Contains(Name))
	{
		const FRotator Rotation = TransformData[EWingMovementAxis::Closeness][Name];
		OutTransform.ConcatenateRotation(Rotation.Quaternion() * Closeness);
	}
	if (TransformData[EWingMovementAxis::FlapAngle].Contains(Name))
	{
		OutTransform.ConcatenateRotation(TransformData[EWingMovementAxis::FlapAngle][Name].Quaternion() * FlapAngle);
	}
	if (TransformData[EWingMovementAxis::TiltAngle].Contains(Name))
	{
		OutTransform.ConcatenateRotation(TransformData[EWingMovementAxis::TiltAngle][Name].Quaternion() * TiltAngle);
	}

	if (FullBoneName.ToString().EndsWith("R"))
	{
		OutTransform.Mirror(EAxis::Y, EAxis::Y);
		OutTransform.Mirror(EAxis::Z, EAxis::Z);
	}
	
	return OutTransform;
}
