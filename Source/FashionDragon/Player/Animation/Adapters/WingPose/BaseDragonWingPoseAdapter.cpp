#include "BaseDragonWingPoseAdapter.h"

enum class EWingMovementAxis
{
	FlapAngle,
	TiltAngle,
	FlightFoldState,
};

FTransform FBaseDragonWingPoseAdapter::ProcessBone(const FName& FullBoneName, const FPoseWingEffector& Effector)
{
	TMap<EWingMovementAxis, TMap<FString, FRotator>> TransformData =
	{
		{EWingMovementAxis::FlapAngle,{
								 
			}
		},
		{EWingMovementAxis::TiltAngle,{
								 
			}
		},
		{
			EWingMovementAxis::FlightFoldState,{
				{ "Wing_001", FRotator(60.0, -20, -20) },
				{ "Wing_002", FRotator(-115.0, 0, 0) },
				{ "Wing_010", FRotator(120.0, 0, 0) },
				{ "Wing_020", FRotator(100.0, 0, 0) },
				{ "Wing_030", FRotator(85.0, 0, 0) },
				{ "Wing_040", FRotator(60.0, 0, 0) },
			}
		},
	};
	
	FTransform OutTransform = FTransform::Identity;
	const FString Name = FullBoneName.ToString().LeftChop(2);

	const float FlapAngle = Effector.FlapAngle;
	const float TiltAngle = Effector.TiltAngle;
	const float FlightFoldState = Effector.FlightFoldState;

	if (TransformData[EWingMovementAxis::FlightFoldState].Contains(Name))
	{
		const FRotator Rotation = TransformData[EWingMovementAxis::FlightFoldState][Name];
		OutTransform.ConcatenateRotation(Rotation.Quaternion() * FlightFoldState);
	}
	if (TransformData[EWingMovementAxis::FlapAngle].Contains(Name))
	{
		OutTransform.ConcatenateRotation(TransformData[EWingMovementAxis::FlapAngle][Name].Quaternion() * FlapAngle);
	}
	if (TransformData[EWingMovementAxis::TiltAngle].Contains(Name))
	{
		OutTransform.ConcatenateRotation(TransformData[EWingMovementAxis::TiltAngle][Name].Quaternion() * TiltAngle);
	}

	OutTransform.NormalizeRotation();

	if (FullBoneName.ToString().EndsWith("R"))
	{
		OutTransform.Mirror(EAxis::Y, EAxis::Y);
		OutTransform.Mirror(EAxis::Z, EAxis::Z);
	}
	
	return OutTransform;
}
