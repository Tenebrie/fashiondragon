class APickup : AActor
{
	UPROPERTY(DefaultComponent, RootComponent)
	USceneComponent SceneRoot;
	
	UPROPERTY(DefaultComponent)
    UStaticMeshComponent PickupMesh;

	UPROPERTY(DefaultComponent)
	UBoxComponent CollisionBox;
	
	// default PickupMesh.RelativeLocation = FVector(0, 0, 250);

	UFUNCTION(BlueprintCallable)
	void OnPickup(FWalletTransaction& Transaction) {}

	FWalletTransaction GetTransaction()
	{
		FWalletTransaction Transaction = FWalletTransaction();
		OnPickup(Transaction);
		return Transaction;
	}

	UFUNCTION(BlueprintOverride)
	void ActorBeginOverlap(AActor OtherActor)
	{
		AScriptedMainCharacter Char = Cast<AScriptedMainCharacter>(OtherActor);
		if (Char == nullptr)
			return;

		Char.Wallet.OnPickup(this);
		DestroyActor();
	}
}
