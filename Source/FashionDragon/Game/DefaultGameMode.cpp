#include "DefaultGameMode.h"

#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Input/DefaultPlayerController.h"

ADefaultGameMode::ADefaultGameMode()
{
	DefaultPawnClass = AMainCharacter::StaticClass();
	PlayerControllerClass = ADefaultPlayerController::StaticClass();
}
