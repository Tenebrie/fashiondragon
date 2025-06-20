class ASubsystemManager : AActor
{
	UFUNCTION(BlueprintOverride)
	void Tick(float DeltaSeconds)
	{
		UPickupProjectileSystem::Get().Tick(DeltaSeconds);
	}
};