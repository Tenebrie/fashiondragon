class UPlayerInteractablesComponent : UPlayerComponent
{
	TArray<AInteractable> ActiveInteractables;

	UFUNCTION(BlueprintOverride)
	void Tick(float DeltaSeconds)
	{
		auto Controller = GetCharacter().GetController();
		auto PlayerController = Cast<APlayerController>(Controller);

		auto MySubsystem = UInteractablesSubsystem::Get(PlayerController);
		auto NearbyInteractables = MySubsystem.GetNearbyInteractables(GetCharacter(), 1000.0f);

		for (auto OldInteractable : ActiveInteractables)
		{
			OldInteractable.HidePrompt();
		}

		ActiveInteractables = NearbyInteractables;
		for (auto NewInteractable : ActiveInteractables)
		{
			NewInteractable.ShowPrompt();
		}
	}

	void TriggerInteract()
	{
		for (auto Interactable : ActiveInteractables)
		{
			Interactable.OnInteract();
		}
	}
};