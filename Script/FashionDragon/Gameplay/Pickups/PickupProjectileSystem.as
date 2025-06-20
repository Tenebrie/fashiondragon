class UPickupProjectileSystem : UWorldSubsystem
{
	TArray<APickupProjectile> Projectiles;

	void Tick(float DeltaSeconds)
	{
		TArray<APickupProjectile> NextFrameProjectiles = TArray<APickupProjectile>();
		for (APickupProjectile Projectile : Projectiles)
		{
			if (Projectile.SplinePosition >= 1.0f && Projectile.EndDelayRemaining <= 0.0f)
			{
				Projectile.DestroyActor();
				continue;
			}

			NextFrameProjectiles.Add(Projectile);

			if (Projectile.StartDelayRemaining > 0.0f)
			{
				Projectile.StartDelayRemaining -= DeltaSeconds;
				continue;
			}

			if (Projectile.AnimationDuration <= 0.01)
				Projectile.SplinePosition = 1.0f;
			else
				Projectile.SplinePosition = Math::Clamp(Projectile.SplinePosition + DeltaSeconds / Projectile.AnimationDuration, 0, 1);
			
			FVector TargetPosition = Projectile.TargetActor.GetActorLocation();
			FVector CurrentPosition = Math::CubicInterp(
				Projectile.StartingPoint,
				FVector(0, 0, 2500),
				TargetPosition,
				FVector(0, 0, 500),
				Projectile.SplinePosition);

			Projectile.SetActorLocation(CurrentPosition);
			if (Projectile.SplinePosition >= 1.0f && !Projectile.bHasImpacted)
			{
				Projectile.bHasImpacted = true;
				Projectile.OnImpact();
			}

			if (Projectile.SplinePosition >= 1.0f && Projectile.EndDelayRemaining > 0.0f)
			{
				Projectile.EndDelayRemaining -= DeltaSeconds;
			}
		}
		Projectiles = NextFrameProjectiles;
	}

	void Register(APickupProjectile Projectile)
	{
		Projectiles.Add(Projectile);
	}
}