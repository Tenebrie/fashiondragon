#pragma once
#include "GameFramework/Character.h"

#include "BaseUnit.generated.h"

UCLASS()
class FASHIONDRAGON_API ABaseUnit : public ACharacter
{
	GENERATED_BODY()

	FTimerHandle NavigationUpdateTimer;

	void OnTimerTick();
	TArray<FVector> PathPoints = {};
	int32 CurrentPathIndex = 0;
	bool bIsFollowingPath = false;
	
public:
	ABaseUnit();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
