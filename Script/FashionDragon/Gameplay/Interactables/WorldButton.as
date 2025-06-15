class AWorldButton : AInteractable
{
	UPROPERTY(DefaultComponent)
	UStaticMeshComponent StandMesh;

	UPROPERTY(DefaultComponent, Attach = StandMesh)
	UStaticMeshComponent ButtonMesh;

	bool bIsPressing = false;
	float PressingOffset = 0;
	float ButtonBaseOffset = 0;

	void OnInteract() override
	{
		Super::OnInteract();
		bIsPressing = true;
	}

	UFUNCTION(BlueprintOverride)
	void BeginPlay()
	{
		Super::BeginPlay();
		ButtonBaseOffset = ButtonMesh.GetRelativeLocation().Z;
	}

	UFUNCTION(BlueprintOverride)
	void Tick(float DeltaSeconds)
	{
		float MovementSpeed = 100.0f;
		float MovementRange = 25;
		if (bIsPressing && PressingOffset < MovementRange)
		{
			PressingOffset += MovementSpeed * DeltaSeconds;
			if (PressingOffset >= MovementRange)
			{
				PressingOffset = MovementRange;
				bIsPressing = false;
			}
		}
		else if (!bIsPressing && PressingOffset > 0)
		{
			PressingOffset -= MovementSpeed * DeltaSeconds;
			if (PressingOffset < 0) { PressingOffset = 0.0f; }
		}

		ButtonMesh.SetRelativeLocation(FVector(0, 0, ButtonBaseOffset - PressingOffset));
	}
};