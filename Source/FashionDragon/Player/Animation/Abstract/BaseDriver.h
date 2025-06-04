#pragma once

#include "FashionDragon/Common/Object.h"

class UDragonAnimInstance;

enum class EDriverBlend
{
	Instant,
	EaseOut,
	AlwaysActive,
};

class FBaseDriver : public FObject
{
protected:
	UDragonAnimInstance* AnimInstance;
	FString Name;
	
	float CyclePosition = 0.0f;
	
	float BlendAlpha = 1.0f;
	float TargetBlendAlpha = 1.0f;
	float BlendSpeed = 1.0f;
	EDriverBlend BlendMode = EDriverBlend::Instant;
	
	float GetRawInputRotation() const;
	float GetInputRotation() const;
	FVector RotateVectorToInputRotation(const FVector& VectorToRotate, const bool Inverted = false) const;

public:
	explicit FBaseDriver(UDragonAnimInstance* AnimInstance): AnimInstance(AnimInstance) {}

	virtual void NativeBeginPlay() {}
	virtual void Tick(const float DeltaTime);
	
	float GetCyclePosition() const { return CyclePosition; }
	void SetCyclePosition(const float NewCyclePosition) { CyclePosition = NewCyclePosition; }

	float GetBlendAlpha() const { return BlendAlpha; }
	virtual void SetDesiredBlendAlpha(const float NewBlendAlpha);
	void ForceSetBlendAlpha(const float NewBlendAlpha) { BlendAlpha = NewBlendAlpha; TargetBlendAlpha = NewBlendAlpha; }

	virtual FString GetDebugState() const { return ""; }
};
