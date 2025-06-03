#pragma once

// All layers are fully virtualized, and their output is summed
UENUM(BlueprintType)
enum class EDriverLayer: uint8
{
	// Main movement drivers (idle, walk, jump for legs)
	Primary = 0,
	// Independent movement components
	Jump = 1,
	Sway = 2,
	Momentum = 3,
	RotateToMovement = 4,

	// Special - do not use
	TotalCount = RotateToMovement + 1,
	Unknown,
	Output,
};
