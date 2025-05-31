// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "MainCharacter.generated.h"

class UFlightHandler;
class URotationInputHandler;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLegPlanted, FVector, SpawnLocation);

enum class EGroundMovementMode
{
	Walking,
	Trotting,
	Sprinting,
};

UCLASS()
class FASHIONDRAGON_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

	EGroundMovementMode MovementMode;
	EGroundMovementMode PreferredMovementMode = EGroundMovementMode::Trotting;

	UPROPERTY() UFlightHandler* FlightHandler;
	UPROPERTY() URotationInputHandler* RotationInputHandler;
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnLegPlanted OnLegPlanted;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* DragonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UPhysicalAnimationComponent* PhysicalAnimation;

private:
	UPROPERTY(EditAnywhere, Category = "Assets", meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* DragonMeshAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* MeshRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* DetachedMeshRoot;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Input callbacks
	void GroundMovement(const FInputActionValue& Value);
	void CameraMove(const FInputActionValue& Value);
	void FlightCameraMove(const FInputActionValue& Value);
	void StartJump();
	void ReleaseJump();
	void StartSprint();
	void StopSprint();
	void TogglePreferredGroundMovement();
	void CastSomeSpell();
	void SwitchGroundMovementMode(EGroundMovementMode NewMode);

public:
	virtual void PostInitializeComponents() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
