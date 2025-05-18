#pragma once

struct FPoseEffectorContext
{
	FPoseEffectorContext(const float DeltaTime, const float BlendAlpha): DeltaTime(DeltaTime), BlendAlpha(BlendAlpha)
	{
	}

	const float DeltaTime;
	const float BlendAlpha;
};
