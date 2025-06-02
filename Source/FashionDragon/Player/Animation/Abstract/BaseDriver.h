#pragma once

class UDragonAnimInstance;

enum class EDriverBlend
{
	Instant,
	EaseOut,
};

#define CLASSNAME(T) const FName GetClassName() const { return FName(#T); }
#define ExpandNamedClass(A, B, C) A B C CLASSNAME(B);
	
ExpandNamedClass(class, FBaseDriver, {)
	UDragonAnimInstance* AnimInstance;
	float CyclePosition = 0.0f;
	float VisualCyclePosition = 0.0f;
	
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
	float GetVisualCyclePosition() const { return VisualCyclePosition; }
	void SetCyclePosition(const float NewCyclePosition) { CyclePosition = NewCyclePosition; }

	float GetBlendAlpha() const { return BlendAlpha; }
	virtual void SetBlendAlpha(const float NewBlendAlpha);

	virtual ~FBaseDriver() = default;
};
