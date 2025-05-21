#pragma once

class UDragonAnimInstance;

class FBaseDriver
{
protected:
	UDragonAnimInstance* AnimInstance;
	float CyclePosition = 0.0f;
	float VisualCyclePosition = 0.0f;
	float BlendAlpha = 1.0f;

	float GetRawInputRotation() const;
	float GetInputRotation() const;
	FVector RotateVectorToInputRotation(const FVector& VectorToRotate, const bool Inverted = false) const;

public:
	explicit FBaseDriver(UDragonAnimInstance* AnimInstance): AnimInstance(AnimInstance) {}
	
	float GetCyclePosition() const { return CyclePosition; }
	float GetVisualCyclePosition() const { return VisualCyclePosition; }

	float GetBlendAlpha() const { return BlendAlpha; }
	virtual void SetBlendAlpha(const float NewBlendAlpha) { BlendAlpha = NewBlendAlpha; }
	
	virtual ~FBaseDriver() = default;
};

namespace EBodyDriverGroup
{
	enum EBodyDriverGroup: uint8
	{
		General = 0,
		Jump = 1,
		Sway = 2,
		Momentum = 3,
	};
}

namespace EHipsDriverGroup
{
	enum EHipsDriverGroup: uint8
	{
		General = 0,
		Sway = 1,
	};
}

