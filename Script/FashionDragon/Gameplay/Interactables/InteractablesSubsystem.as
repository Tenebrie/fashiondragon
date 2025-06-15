class UInteractablesSubsystem : UScriptLocalPlayerSubsystem
{
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	TArray<AInteractable> GetNearbyInteractables(AActor CenterActor, float Radius) const
	{
		TArray<AInteractable> NearbyInteractables;
		TArray<AActor> FoundActors;
		
		// Get all interactables in the world
		GetAllActorsOfClass(AInteractable::StaticClass(), FoundActors);
		
		// Filter by distance
		FVector CenterLocation = CenterActor.GetActorLocation();
		for (AActor Actor : FoundActors)
		{
			float Distance = (Actor.GetActorLocation() - CenterLocation).Size();
			if (Distance <= Radius)
			{
				NearbyInteractables.Add(Cast<AInteractable>(Actor));
			}
		}
		
		return NearbyInteractables;
	}
} 