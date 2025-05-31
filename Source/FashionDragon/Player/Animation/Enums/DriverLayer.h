#pragma once

// All layers are fully virtualized, and their output is summed
enum class EDriverLayer: uint8
{
	// Main movement drivers (idle, walk, jump for legs)
	Primary = 0,
	// Independent movement components
	Jump = 1,
	Sway = 2,
	Momentum = 3,
	LegPlacement = 4,
	RotateToMovement = 5,
	WalkBob = 6,
};
