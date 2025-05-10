#pragma once

struct FPoseEffector
{
	mutable FVector Position = FVector(0.0f, 0.0f, 0.0f);
	mutable FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
};
