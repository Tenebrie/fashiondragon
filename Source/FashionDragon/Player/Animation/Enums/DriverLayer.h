#pragma once

// All layers are fully virtualized, and their output is summed
UENUM(BlueprintType)
enum class EDriverLayer: uint8
{
	// Main movement drivers (idle, walk, jump for legs)
	Primary = 0,
	// Independent movement components
	Constant = 1,
	Jump = 2,
	Sway = 3,
	Momentum = 4,
	RotateToMovement = 5,

	// Special - do not use
	TotalCount = RotateToMovement + 1,
	Unknown,
	Output,
};
