class AInteractable : AActor
{
	UPROPERTY(DefaultComponent, RootComponent)
	USceneComponent Root;

	UPROPERTY(DefaultComponent)
	UWidgetComponent PromptWidget;

	default PromptWidget.WidgetSpace = EWidgetSpace::World;

	UFUNCTION(BlueprintOverride)
	void BeginPlay()
	{
		PromptWidget.SetVisibility(false);
	}

	UFUNCTION()
	void OnInteract()
	{
		TArray<UInteractableComponent> Interactables = GetComponentsByClass(UInteractableComponent);
		for (UInteractableComponent Interactable : Interactables)
		{
			Interactable.OnInteract();
		}
	}

	UFUNCTION()
	void ShowPrompt()
	{
		PromptWidget.SetVisibility(true);
	}

	UFUNCTION()
	void HidePrompt()
	{
		PromptWidget.SetVisibility(false);
	}
};