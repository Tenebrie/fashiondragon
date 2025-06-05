#include "WalkCyclePoseComponent.h"

#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralLegSteppingDriver.h"
#include "FashionDragon/Player/Animation/Abstract/ProceduralPose.h"

void FWalkCyclePoseComponent::SetCycleBreakpoints(const TArray<FBreakpoint>& Breakpoints)
{
	LeftCyclePosition = 0.0f;

	CycleDuration = 0.0f;
	CycleBreakpoints = TArray<FBreakpoint>();
	for (FBreakpoint Tuple : Breakpoints)
	{
		CycleDuration += Tuple.Key;
		CycleBreakpoints.Add(FBreakpoint(CycleDuration, Tuple.Value));
	}
	RightCyclePosition = CycleDuration / 2.0f;
}

void FWalkCyclePoseComponent::Mirror()
{
	LeftCyclePosition += RightCyclePosition;
	RightCyclePosition = LeftCyclePosition - RightCyclePosition;
	LeftCyclePosition -= RightCyclePosition;
}

int FWalkCyclePoseComponent::CheckForBreakpoint(const float Position, const int StateIndex, FProceduralLegSteppingDriver* Driver)
{
	if (CycleBreakpoints.Num() == 0) { return StateIndex; }
	
	if (StateIndex > -1)
	{
		const TPair CurrentBreakpoint = CycleBreakpoints[StateIndex];
		if (Position <= CurrentBreakpoint.Key) { return StateIndex; }
	}

	int BreakpointIndex = 0;
	FBreakpoint LastBreakpoint = CycleBreakpoints[0];
	for (int i = 0; i < CycleBreakpoints.Num(); i++)
	{
		if (Position < CycleBreakpoints[i].Key) { break; }

		if (CycleBreakpoints.Num() >= i + 2)
		{
			LastBreakpoint = CycleBreakpoints[i + 1];
			BreakpointIndex = i + 1;
		}
		else
		{
			LastBreakpoint = CycleBreakpoints[0];
			BreakpointIndex = 0;
		}
	}

	if (StateIndex == BreakpointIndex)
	{
		return StateIndex;
	}

	if (LastBreakpoint.Value == ELegWalkingState::Planted)
	{
		Driver->LockToWorldGround();
	}
	else
	{
		Driver->SetWalkingState(LastBreakpoint.Value);
	}
	return BreakpointIndex;
}

void FWalkCyclePoseComponent::SyncStateFrom(const FWalkCyclePoseComponent* Other)
{
	if (Other->LeftLegDriver->WalkingState == ELegWalkingState::Planted)
	{
		LeftCyclePosition = (Other->LeftCyclePosition / Other->CycleDuration) * CycleDuration;
		RightCyclePosition = LeftCyclePosition + CycleDuration / 2.0f;
	}
	else
	{
		RightCyclePosition = (Other->RightCyclePosition / Other->CycleDuration) * CycleDuration;
		LeftCyclePosition = RightCyclePosition + CycleDuration / 2.0f;
	}

	if (LeftCyclePosition >= CycleDuration)
		LeftCyclePosition -= CycleDuration;
	if (RightCyclePosition >= CycleDuration)
		RightCyclePosition -= CycleDuration;

	LeftState = CheckForBreakpoint(LeftCyclePosition, -1, LeftLegDriver);
	RightState = CheckForBreakpoint(RightCyclePosition, -1, RightLegDriver);
	// LeftLegDriver->SetPositionFrom(Other->LeftLegDriver->GetPositionFrom());
	// LeftLegDriver->SetRotationFrom(Other->LeftLegDriver->GetRotationFrom());
	// RightLegDriver->SetPositionFrom(Other->RightLegDriver->GetPositionFrom());
	// RightLegDriver->SetRotationFrom(Other->RightLegDriver->GetRotationFrom());
	LeftLegDriver->SetCyclePosition(LeftCyclePosition);
	RightLegDriver->SetCyclePosition(RightCyclePosition);
}

void FWalkCyclePoseComponent::ResetState()
{
	if (TimeSinceLastSync <= 0.5f)
	{
		TimeSinceLastSync = 0.0f;
		return;
	}
	
	if (SwitchStartingLeg)
	{
		LeftCyclePosition = 0.0f;
		RightCyclePosition = CycleDuration / 2.0f;
	}
	else
	{
		LeftCyclePosition = CycleDuration / 2.0f;
		RightCyclePosition = 0.0f;
	}
	SwitchStartingLeg = !SwitchStartingLeg;
	TimeSinceLastSync = 0.0f;

	LeftState = CheckForBreakpoint(LeftCyclePosition, -1, LeftLegDriver);
	RightState = CheckForBreakpoint(RightCyclePosition, -1, RightLegDriver);
}

void FWalkCyclePoseComponent::Tick(const float DeltaTime)
{
	FProceduralPoseComponent::Tick(DeltaTime);

	TimeSinceLastSync += DeltaTime;
	if (LeftLegDriver->GetBlendAlpha() <= 0.0f && RightLegDriver->GetBlendAlpha() <= 0.0f)
		return;

	const auto OwningActor = Cast<AMainCharacter>(Pose->AnimInstance->GetOwningActor());
	const auto MovementSpeed = OwningActor->GetVelocity().Size();

	const float AdvanceValue = DeltaTime + MovementSpeed * 0.001f * DeltaTime;
	LeftCyclePosition += AdvanceValue;
	RightCyclePosition += AdvanceValue;

	if (LeftCyclePosition >= CycleDuration)
	{
		LeftCyclePosition -= CycleDuration;
		LeftState = -1;
	}
	if (RightCyclePosition >= CycleDuration)
	{
		RightCyclePosition -= CycleDuration;
		RightState = -1;
	}
	
	LeftState = CheckForBreakpoint(LeftCyclePosition, LeftState, LeftLegDriver);
	RightState = CheckForBreakpoint(RightCyclePosition, RightState, RightLegDriver);
}
