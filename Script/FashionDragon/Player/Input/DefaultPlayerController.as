class AScriptedPlayerController : ADefaultPlayerController
{
	UPROPERTY()
	TSubclassOf<UPlayerWalletWidget> WalletWidget;

	AMainCharacter GetCharacter()
	{
		return Cast<AMainCharacter>(GetControlledPawn());
	}

	UFUNCTION(BlueprintOverride)
	void OnInteract()
	{
		GetCharacter().GetComponent(UPlayerInteractablesComponent).TriggerInteract();
	}

	UFUNCTION(BlueprintOverride)
	void BeginPlay()
	{
		auto Widget = NewObject(this, WalletWidget, n"WalletWidget");
		Widget.AddToViewport();
		GetCharacter().GetComponent(UPlayerWallet).Widget = Widget;
	}
}