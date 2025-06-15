class AScriptedMainCharacter : AMainCharacter
{
	UPROPERTY(DefaultComponent)
	UPlayerWallet Wallet;

	UPROPERTY(DefaultComponent)
	UDragonFlightHandler FlightHandler;

	// UPROPERTY(DefaultComponent)
	// USnekComponent Snek;

	UPROPERTY(DefaultComponent)
	UPlayerInteractablesComponent InteractablesComponent;
}