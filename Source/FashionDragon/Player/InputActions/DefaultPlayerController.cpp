#include "DefaultPlayerController.h"

#include "Actions.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ADefaultPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer) { return; }

	const auto LocalInputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!LocalInputSubsystem) { return; }

	InputSubsystem = LocalInputSubsystem;

	SetupInputMappings();
	SetControlMode(EControlMode::Ground);
	InputSubsystem->AddMappingContext(DebugInputContext, 0);
}

void ADefaultPlayerController::SetupInputMappings()
{
	DragonGroundInputContext = MakeInputContext();
	{
		const auto Context = DragonGroundInputContext;
		AddMapping(Context, UActions::GroundMovement(), EKeys::W).Swizzle();
		AddMapping(Context, UActions::GroundMovement(), EKeys::A).Negate();
		AddMapping(Context, UActions::GroundMovement(), EKeys::S).Swizzle().Negate();
		AddMapping(Context, UActions::GroundMovement(), EKeys::D);
		AddMapping(Context, UActions::GroundMovement(), EKeys::Gamepad_Left2D).DeadZone(0.1f);

		AddMapping(Context, UActions::CameraMove(), EKeys::MouseX);
		AddMapping(Context, UActions::CameraMove(), EKeys::MouseY).Swizzle();
		AddMapping(Context, UActions::CameraMove(), EKeys::Gamepad_RightX).DeadZone(0.1f);
		AddMapping(Context, UActions::CameraMove(), EKeys::Gamepad_RightY).Swizzle().Negate().DeadZone(0.1f);

		AddMapping(Context, UActions::Jump(), EKeys::SpaceBar);
		AddMapping(Context, UActions::Jump(), EKeys::Gamepad_FaceButton_Bottom);

		AddMapping(Context, UActions::Sprint(), EKeys::LeftShift);
		AddMapping(Context, UActions::Sprint(), EKeys::Gamepad_LeftThumbstick);

		AddMapping(Context, UActions::TogglePreferredMovement(), EKeys::CapsLock);

		AddMapping(Context, UActions::CastASpell(), EKeys::Q);
	}

	DragonFlyingInputContext = MakeInputContext();
	{
		const auto Context = DragonFlyingInputContext;
		AddMapping(Context, UActions::FlightCamera(), EKeys::MouseX);
		AddMapping(Context, UActions::FlightCamera(), EKeys::MouseY).Swizzle();
		AddMapping(Context, UActions::FlightCamera(), EKeys::Gamepad_RightX).DeadZone(0.1f);
		AddMapping(Context, UActions::FlightCamera(), EKeys::Gamepad_RightY).Swizzle().Negate().DeadZone(0.1f);
	}

	DebugInputContext = MakeInputContext();
	{
		const auto Context = DebugInputContext;
		AddMapping(Context, UActions::ShowAnimDebug(), EKeys::NumPadOne);
	}
}

void ADefaultPlayerController::SetControlMode(const EControlMode Mode) const
{
	if (!InputSubsystem) { return; }

	if (Mode == EControlMode::Ground)
	{
		InputSubsystem->RemoveMappingContext(DragonFlyingInputContext);
		InputSubsystem->AddMappingContext(DragonGroundInputContext, 1);
	}
	else if (Mode == EControlMode::Flying)
	{
		InputSubsystem->RemoveMappingContext(DragonGroundInputContext);
		InputSubsystem->AddMappingContext(DragonFlyingInputContext, 1);
	}
	else
	{
		InputSubsystem->RemoveMappingContext(DragonGroundInputContext);
		InputSubsystem->RemoveMappingContext(DragonFlyingInputContext);
	}
}
