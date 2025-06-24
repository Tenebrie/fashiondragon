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

	UFUNCTION(BlueprintOverride)
	void OnFlight_Ability_01()
	{
		GetCharacter().GetMovementComponent().Velocity += GetCharacter().DetachedMeshRoot.ForwardVector.GetSafeNormal() * 5000.0f;
	}
}