#pragma once
#include "FashionDragon/Player/Generics/PlayerComponent.h"
#include "AnimationDebugReporter.generated.h"

class AMainCharacter;

USTRUCT(BlueprintType)
struct FDriverDebugInfo
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly) float BlendAlpha = 0.0f;
	UPROPERTY(BlueprintReadOnly) FString Name = TEXT("Unknown Driver");
	UPROPERTY(BlueprintReadOnly) FVector Position = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly) FRotator Rotation = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadOnly) FVector PositionDelta = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly) FRotator RotationDelta = FRotator::ZeroRotator;
};

USTRUCT(BlueprintType)
struct FPoseDebugInfo
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FDriverDebugInfo> Drivers;
};


UCLASS()
class FASHIONDRAGON_API UAnimationDebugReporter : public UPlayerComponent
{
	GENERATED_BODY()
	
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void CollectInfo();
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FPoseDebugInfo> PoseDebugInfo;
};