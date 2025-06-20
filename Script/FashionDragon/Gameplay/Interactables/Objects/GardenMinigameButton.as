class AGardenMinigameButton : AWorldButton
{
	UPROPERTY()
	ATriggerBox GardeningArea;

	UPROPERTY()
	TSubclassOf<APickup_GardenFlower> FlowerToSpawn;

	UPROPERTY()
	TSubclassOf<APickupProjectile> PickupProjectile;

	UPROPERTY()
	UStaticMesh FlowerMesh;

	UPROPERTY(DefaultComponent)
	UHierarchicalInstancedStaticMeshComponent FlowersHISM;

	UPROPERTY()
	TMap<int, float> FadingOutInstances;

	UFUNCTION(BlueprintOverride)
	void BeginPlay()
	{
		Super::BeginPlay();
		FlowersHISM.StaticMesh = FlowerMesh;

		FlowersHISM.SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		FlowersHISM.SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		FlowersHISM.SetGenerateOverlapEvents(true);
		FlowersHISM.bMultiBodyOverlap = true;
		// 0 = Fade Out
		FlowersHISM.NumCustomDataFloats = 1;

		FlowersHISM.OnComponentBeginOverlap.AddUFunction(this, n"OnFlowerOverlap");
	}

	UFUNCTION(BlueprintOverride)
	void Tick(float DeltaSeconds)
	{
		Super::Tick(DeltaSeconds);
		for (auto Instance : FadingOutInstances)
		{
			if (Instance.Value >= 1.0f) { continue; }
			Instance.Value = Math::Clamp(Instance.Value + DeltaSeconds * 2.0f, 0.0f, 1.0f);
			FlowersHISM.SetCustomDataValue(Instance.Key, 0, Instance.Value);
		}
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
		if (Character == nullptr || FadingOutInstances.Contains(SweepResult.Item))
		{
			return;
		}

		FadingOutInstances.Add(SweepResult.Item, 0.0f);
		auto Transaction = FWalletTransaction();
		Transaction.Flowers = 1;
		// Character.Wallet.Add(Transaction);
		FTransform InstanceTransform;
		FlowersHISM.GetInstanceTransform(SweepResult.Item, InstanceTransform, true);
		auto Projectile = SpawnActor(
			PickupProjectile,
			InstanceTransform.Location + FlowersHISM.StaticMesh.BoundingBox.Center,
			FRotator::ZeroRotator,
			n"GardenMinigamePickupProjectile"
		);
		Projectile.TargetActor = Character;
		Projectile.Transaction = Transaction;
		Projectile.StartDelayRemaining = 0.2f;
		Projectile.AnimationDuration = 0.7f;
		Projectile.EndDelayRemaining = 0.5f;
	}

	void OnInteract() override
	{
		Super::OnInteract();

		FlowersHISM.ClearInstances();
		FadingOutInstances.Empty();

		FVector Position;
		FVector Bounds;
		FHitResult Hit;

		TArray<AActor> SpawnedFlowers = TArray<AActor>();
		GardeningArea.GetActorBounds(false, Position, Bounds);
		
		for (int i = 0; i < 500; i++)
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
			int Index = FlowersHISM.AddInstance(InstanceTransform, true);
			FlowersHISM.SetCustomDataValue(Index, 0, 0.0f);
		}
	}
}