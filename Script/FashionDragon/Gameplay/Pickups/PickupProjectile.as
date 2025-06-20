class APickupProjectile : AActor
{
	UPROPERTY(DefaultComponent, RootComponent)
	USceneComponent Root;

	UPROPERTY(DefaultComponent)
	UNiagaraComponent Emitter;

	UPROPERTY()
	UNiagaraSystem ImpactEmitter;
	bool bHasImpacted = false;

	AScriptedMainCharacter TargetActor;
	float AnimationDuration = 1.0f;
	float SplinePosition = 0.0f;
	float StartDelayRemaining = 0.0f;
	float EndDelayRemaining = 0.0f;
	FWalletTransaction Transaction;
	FVector StartingPoint;

	UFUNCTION(BlueprintOverride)
	void BeginPlay()
	{
		StartingPoint = GetActorLocation();
		UPickupProjectileSystem::Get().Register(this);
		SetActorTickEnabled(false);
	}

	UFUNCTION()
	void OnImpact()
	{
		// if (ImpactEmitter != nullptr)
		// {
		// 	Niagara::SpawnSystemAtLocation(
		// 		ImpactEmitter,
		// 		GetActorLocation(),
		// 		FRotator::ZeroRotator,
		// 		FVector(1, 1, 1),
		// 		true,
		// 		true,
		// 		ENCPoolMethod::AutoRelease
		// 	);
		// }
		TargetActor.Wallet.Add(Transaction);
	}
}