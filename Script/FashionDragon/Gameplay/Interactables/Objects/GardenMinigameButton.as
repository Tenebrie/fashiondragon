class AGardenMinigameButton : AWorldButton
{
	UPROPERTY()
	ATriggerBox GardeningArea;

	UPROPERTY()
	TSubclassOf<APickup_GardenFlower> FlowerToSpawn;

	UPROPERTY()
	UStaticMesh FlowerMesh;

	UPROPERTY(DefaultComponent)
	UHierarchicalInstancedStaticMeshComponent FlowersHISM;

	UFUNCTION(BlueprintOverride)
	void BeginPlay()
	{
		Super::BeginPlay();
		FlowersHISM.StaticMesh = FlowerMesh;

		FlowersHISM.SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		FlowersHISM.SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		FlowersHISM.SetGenerateOverlapEvents(true);
		FlowersHISM.bMultiBodyOverlap = true;

		FlowersHISM.OnComponentBeginOverlap.AddUFunction(this, n"OnFlowerOverlap");
	}

	UFUNCTION()
	void OnFlowerOverlap(
		UPrimitiveComponent OverlappedComponent,
		AActor OtherActor,
		UPrimitiveComponent OtherComp,
		int OtherBodyIndex,
		bool bFromSweep,
		const FHitResult&in SweepResult)
	{
		AScriptedMainCharacter Character = Cast<AScriptedMainCharacter>(OtherActor);
		if (Character == nullptr)
		{
			return;
		}
		// Print("" + SweepResult.Item);

		FlowersHISM.RemoveInstance(SweepResult.Item);
		auto Transaction = FWalletTransaction();
		Transaction.Flowers = 1;
		Character.Wallet.Add(Transaction);
	}

	void OnInteract() override
	{
		Super::OnInteract();

		FlowersHISM.ClearInstances();

		FVector Position;
		FVector Bounds;
		FHitResult Hit;

		TArray<AActor> SpawnedFlowers = TArray<AActor>();
		GardeningArea.GetActorBounds(false, Position, Bounds);
		
		for (int i = 0; i < 2000; i++)
		{
			FVector TargetPoint = Math::RandomPointInBoundingBox(Position, Bounds);

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActors(SpawnedFlowers);
			System::LineTraceSingleByChannel(Hit, TargetPoint, TargetPoint + FVector(0, 0, -1000), ECollisionChannel::ECC_WorldStatic, QueryParams);
			if (Hit.bBlockingHit)
			{
				TargetPoint = Hit.Location;
			}
			FRotator FlowerRotation = FRotator(0, Math::RandRange(0, 360), 0);
			FTransform InstanceTransform = FTransform(FlowerRotation, TargetPoint, FVector(2, 2, 2));
			FlowersHISM.AddInstance(InstanceTransform, true);
		}
	}
}