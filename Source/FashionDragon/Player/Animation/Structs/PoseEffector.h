#pragma once

struct FPoseEffector
{
	FVector Position = FVector(0.0f, 0.0f, 0.0f);
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);

	FPoseEffector() {}
	FPoseEffector(FPoseEffector const&) = default;
	FPoseEffector(const FVector& Position, const FRotator& Rotation): Position(Position), Rotation(Rotation) {}

	FPoseEffector AddPosition(const FVector& Delta) const
	{
		return FPoseEffector(Position + Delta, Rotation);
	}
	FPoseEffector AddRotation(const FRotator& Delta) const
	{
		return FPoseEffector(Position, Rotation + Delta);
	}
	FPoseEffector SetPosition(const FVector& NewPosition) const
	{
		return FPoseEffector(NewPosition, Rotation);
	}
	FPoseEffector SetRotation(const FRotator& NewRotation) const
	{
		return FPoseEffector(Position, NewRotation);
	}
};
