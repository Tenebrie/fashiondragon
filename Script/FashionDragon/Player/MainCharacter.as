class AScriptedMainCharacter : AMainCharacter
{
	UPROPERTY(DefaultComponent)
	UPlayerWallet Wallet;

	UPROPERTY(DefaultComponent)
	UPlayerInteractablesComponent InteractablesComponent;

	UFUNCTION(BlueprintOverride)
	void BeginPlay()
	{
		FlightHandler = CreateComponent(UScriptedFlightHandler, n"Flight Handler");
	}
}