#include "DefaultPlayerController.h"

#include "Actions.h"
#include "EnhancedInputSubsystems.h"

void ADefaultPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer) { return; }

	const auto InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!InputSubsystem) { return; }

	SetupInputMappings();
	InputSubsystem->AddMappingContext(DragonGroundInputContext, 0);
}

void ADefaultPlayerController::SetupInputMappings()
{
	DragonGroundInputContext = MakeInputContext();
	{
		const auto Context = DragonGroundInputContext;
		AddMapping(Context, UActions::GroundMovement, EKeys::W).Swizzle();
		AddMapping(Context, UActions::GroundMovement, EKeys::A).Negate();
		AddMapping(Context, UActions::GroundMovement, EKeys::S).Swizzle().Negate();
		AddMapping(Context, UActions::GroundMovement, EKeys::D);
		AddMapping(Context, UActions::GroundMovement, EKeys::Gamepad_Left2D).DeadZone(0.1f);

		AddMapping(Context, UActions::CameraMove, EKeys::MouseX);
		AddMapping(Context, UActions::CameraMove, EKeys::MouseY).Swizzle();
		AddMapping(Context, UActions::CameraMove, EKeys::Gamepad_RightX).DeadZone(0.1f);
		AddMapping(Context, UActions::CameraMove, EKeys::Gamepad_RightY).Swizzle().Negate().DeadZone(0.1f);

		AddMapping(Context, UActions::Jump, EKeys::SpaceBar);
		AddMapping(Context, UActions::Jump, EKeys::Gamepad_FaceButton_Bottom);

		AddMapping(Context, UActions::Sprint, EKeys::LeftShift);
		AddMapping(Context, UActions::Sprint, EKeys::Gamepad_LeftThumbstick);

		AddMapping(Context, UActions::TogglePreferredMovement, EKeys::CapsLock);

		AddMapping(Context, UActions::CastASpell, EKeys::Q);
	}

	DragonFlyingInputContext = MakeInputContext();
}
