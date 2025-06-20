class UScriptedFlightHandler : UFlightHandler
{
	// TODO: Move to C++ as this value needs to be read by animations
	UPROPERTY()
	float WingAreaFactor = 1.00f;

	UPROPERTY() float BaselineDrag = 0.01f;

	UPROPERTY() float WingSpan = 20.0f; // m
	UPROPERTY() float WingArea = 80.0f; // m^2
	UPROPERTY() float Mass = 1000.0f; // kg

	FVector Velocity;
	FVector Acceleration;

	UFUNCTION(BlueprintOverride)
	void Tick(float DeltaTime)
	{
		if (!IsInFlight)
		{
			return;
		}

		auto MovementComponent = GetCharacter().GetMovementComponent();
		Velocity = MovementComponent.Velocity / 100;
		Acceleration = FVector::ZeroVector;

		ApplyWingLift();
		ApplyGravity();
		CheckForGround();

		MovementComponent.Velocity += Acceleration * DeltaTime * 100;
		Print("Velocity = " + MovementComponent.Velocity);
	}

	void ApplyWingLift()
	{
		FVector WorldWindVector = FVector(3, 0, 0); // m/s, doesn't need any conversion
		FVector Up = GetCharacter().DetachedMeshRoot.GetWorldRotation().UpVector;
		FVector Right = GetCharacter().DetachedMeshRoot.GetWorldRotation().RightVector;
		FVector Forward = GetCharacter().DetachedMeshRoot.GetWorldRotation().ForwardVector;

		FVector RelativeVelocity = Velocity - WorldWindVector;
		FVector RelativeWind = -RelativeVelocity.GetSafeNormal();

		if (RelativeWind.IsNearlyZero()) { return; }

		// Lift
		float Vx = RelativeWind.DotProduct(Forward);
		float Vz = RelativeWind.DotProduct(Up);
		// Print("Vz " + Vz + " Vx " + Vx);

		float AngleOfAttack = Math::RadiansToDegrees(Math::Atan2(Vz, -Vx));

		float AirDensity = 1.225f; // kg/m^3
		float DynamicAccelerationPrefactor = 0.5f * AirDensity * WingAreaFactor * WingArea / Mass;
		// float DynamicAccelerationPrefactor = 1.0f;

		float LiftCoefficient = GetLiftCurveValue(AngleOfAttack, 0.11f);
		Print("AoA" + AngleOfAttack + " Cl " + LiftCoefficient);
		float LiftMagnitude = DynamicAccelerationPrefactor * LiftCoefficient * RelativeVelocity.SizeSquared();

		FVector LiftDirection = Right.CrossProduct(RelativeWind).GetSafeNormal();
		Print("Lift" + LiftDirection + " Wind " + RelativeWind + " Test " + LiftDirection.DotProduct(Up));

		Acceleration += LiftDirection * LiftMagnitude;

		// Drag
		float DragCoefficient = GetLiftCurveValue(AngleOfAttack, 0.11f);
		float AR              = (WingSpan * WingSpan) / WingArea;
		float InducedDrag     = (DragCoefficient * DragCoefficient) / (PI * AR * 0.8f);
		float StallDragFactor = 0.3f * Math::Clamp((Math::Abs(AngleOfAttack) - 12.f) / 10.f, 0.f, 1.f);
		float TotalDrag = BaselineDrag + InducedDrag + StallDragFactor;

		float DragMagnitude = DynamicAccelerationPrefactor * RelativeVelocity.SizeSquared() * TotalDrag;
		FVector DragDirection = -RelativeVelocity.GetSafeNormal();
		Print("AR " + DynamicAccelerationPrefactor);

		Acceleration += DragDirection * DragMagnitude;
	}

	void ApplyGravity()
	{
		Acceleration += FVector(0, 0, -9.8);
	}

	void CheckForGround()
	{
		auto CapsuleComponent = GetCharacter().GetComponentByClass(UCapsuleComponent);
		FVector CapsuleLocation = CapsuleComponent.GetWorldLocation();
		FVector CapsuleBottom = FVector(CapsuleLocation.X, CapsuleLocation.Y, CapsuleLocation.Z - CapsuleComponent.CapsuleHalfHeight);
		
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetCharacter());
		FHitResult Hit;
		System::LineTraceSingleByChannel(Hit, CapsuleBottom, CapsuleBottom - FVector(0, 0, 150), ECollisionChannel::ECC_WorldStatic, QueryParams);

		if (Hit.bBlockingHit)
		{
			GetCharacter().CancelFlight();
		}
	}

	float GetLiftCurveValue(float AngleOfAttack, float CoefficientPerDegree)
	{
		float ControlSector = 15; // Degrees in either direction
		float PositiveStallCutoff = 0.0f;
		float NegativeStallCutoff = -0.8f;

		// "Core" region
		if (AngleOfAttack >= -ControlSector && AngleOfAttack <= ControlSector)
		{
			return AngleOfAttack * CoefficientPerDegree;
		}

		// Nose too high up
		if (AngleOfAttack > ControlSector)
		{
			return Math::Max((2 * ControlSector - AngleOfAttack) * CoefficientPerDegree, PositiveStallCutoff);
		}

		if (AngleOfAttack < -ControlSector)
		{
			return Math::Min((-2 * ControlSector - AngleOfAttack) * CoefficientPerDegree, NegativeStallCutoff);
		}
		return 0.0f;
	}
};