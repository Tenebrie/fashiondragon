class AGardenMinigameButton : AWorldButton
{
	UPROPERTY()
	ATriggerBox GardeningArea;

	UPROPERTY()
	TSubclassOf<APickup_GardenFlower> FlowerToSpawn;

	void OnInteract() override
	{
		Super::OnInteract();

		TArray<AActor> ActorsInGarden;
		GardeningArea.GetOverlappingActors(ActorsInGarden, APickup_GardenFlower::StaticClass());

		for (auto Actor : ActorsInGarden)
		{
			Actor.DestroyActor();
		}

		FVector Position;
		FVector Bounds;
		FHitResult Hit;

		TArray<AActor> SpawnedFlowers = TArray<AActor>();
		
		for (int i = 0; i < 1500; i++)
		{
			GardeningArea.GetActorBounds(false, Position, Bounds);
			FVector TargetPoint = Math::RandomPointInBoundingBox(Position, Bounds);

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActors(SpawnedFlowers);
			System::LineTraceSingleByChannel(Hit, TargetPoint, TargetPoint + FVector(0, 0, -1000), ECollisionChannel::ECC_WorldStatic, QueryParams);
			if (Hit.bBlockingHit)
			{
				TargetPoint = Hit.Location;
			}
			AActor Actor = SpawnActor(FlowerToSpawn, TargetPoint, FRotator(0, Math::RandRange(0, 360), 0));
			SpawnedFlowers.Add(Actor);
		}
	}
}