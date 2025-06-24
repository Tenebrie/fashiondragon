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
	InputSubsystem->AddMappingContext(CommonInputContext, 0);
	InputSubsystem->AddMappingContext(DebugInputContext, 0);
}

void ADefaultPlayerController::SetupInputMappings()
{
	CommonInputContext = MakeInputContext();
	{
		const auto Context = CommonInputContext;
		AddMapping(Context, UActions::CameraMove(), EKeys::MouseX);
		AddMapping(Context, UActions::CameraMove(), EKeys::MouseY).Swizzle();
		AddMapping(Context, UActions::CameraMove(), EKeys::Gamepad_RightX).RadialDeadZone(0.15f).PerAxisDeadZone(0.04f);
		AddMapping(Context, UActions::CameraMove(), EKeys::Gamepad_RightY).Swizzle().Negate().RadialDeadZone(0.15f).PerAxisDeadZone(0.04f);
		
		AddMapping(Context, UActions::HoldAimDownSights(), EKeys::RightMouseButton);
		AddMapping(Context, UActions::HoldAimDownSights(), EKeys::Gamepad_LeftTrigger);
	}
	
	DragonGroundInputContext = MakeInputContext();
	{
		const auto Context = DragonGroundInputContext;
		AddMapping(Context, UActions::GroundMovement(), EKeys::W).Swizzle();
		AddMapping(Context, UActions::GroundMovement(), EKeys::A).Negate();
		AddMapping(Context, UActions::GroundMovement(), EKeys::S).Swizzle().Negate();
		AddMapping(Context, UActions::GroundMovement(), EKeys::D);
		AddMapping(Context, UActions::GroundMovement(), EKeys::Gamepad_Left2D).RadialDeadZone(0.2f).PerAxisDeadZone(0.1f);

		AddMapping(Context, UActions::Interact(), EKeys::F);
		AddMapping(Context, UActions::Interact(), EKeys::Gamepad_FaceButton_Left);

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

		AddMapping(Context, UActions::Flight_Cancel(), EKeys::SpaceBar);
		AddMapping(Context, UActions::Flight_Cancel(), EKeys::Gamepad_FaceButton_Bottom);

		AddMapping(Context, UActions::Flight_FlapForward(), EKeys::SpaceBar);
		AddMapping(Context, UActions::Flight_Pitch(), EKeys::W);
		AddMapping(Context, UActions::Flight_Pitch(), EKeys::S).Negate();
		AddMapping(Context, UActions::Flight_Yaw(), EKeys::Q).Negate();
		AddMapping(Context, UActions::Flight_Yaw(), EKeys::E);
		AddMapping(Context, UActions::Flight_Roll(), EKeys::A).Negate();
		AddMapping(Context, UActions::Flight_Roll(), EKeys::D);
		AddMapping(Context, UActions::Flight_FoldWings(), EKeys::LeftShift);

		AddMapping(Context, UActions::Flight_Ability_01(), EKeys::One);
		AddMapping(Context, UActions::Flight_Ability_02(), EKeys::Two);
		AddMapping(Context, UActions::Flight_Ability_03(), EKeys::Three);
		AddMapping(Context, UActions::Flight_Ability_04(), EKeys::Four);
	}

	DebugInputContext = MakeInputContext();
	{
		const auto Context = DebugInputContext;
		AddMapping(Context, UActions::ShowAnimDebug(), EKeys::NumPadOne);
		AddMapping(Context, UActions::DebugButtonNum2(), EKeys::NumPadTwo);
		AddMapping(Context, UActions::DebugButtonNum3(), EKeys::NumPadThree);
	}
}

void ADefaultPlayerController::SetControlMode(const EControlMode Mode) const
{
	if (!InputSubsystem) { return; }

	if (Mode == EControlMode::Ground)
	{
		InputSubsystem->RemoveMappingContext(DragonFlyingInputContext);
		InputSubsystem->AddMappingContext(DragonGroundInputContext, 2);
	}
	else if (Mode == EControlMode::Flying)
	{
		InputSubsystem->RemoveMappingContext(DragonGroundInputContext);
		InputSubsystem->AddMappingContext(DragonFlyingInputContext, 2);
	}
	else
	{
		InputSubsystem->RemoveMappingContext(DragonGroundInputContext);
		InputSubsystem->RemoveMappingContext(DragonFlyingInputContext);
	}
}
