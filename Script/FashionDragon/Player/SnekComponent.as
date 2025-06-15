class USnekComponent : UPlayerComponent
{
	APickup_Bling Target;

	UPROPERTY()
	TSubclassOf<APickup_Bling> PickupBlingClass;

	UFUNCTION(BlueprintOverride)
	void Tick(float DeltaSeconds)
	{
		if (IsValid(Target))
			return;

        for (int i = 0; i < 10; i++)
		{
            FVector SpawnLocation = GetCharacter().GetActorLocation() + FVector(2000 * Math::RandRange(-1, 1), 2000 * Math::RandRange(-1, 1), 0);

		    Target = SpawnActor(PickupBlingClass, SpawnLocation);
		}
	}
}
