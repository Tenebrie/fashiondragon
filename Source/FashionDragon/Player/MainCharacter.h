// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAnimationDebugReporter;
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

	FRotator DesiredFacingRotation = FRotator::ZeroRotator;

	EGroundMovementMode MovementMode;
	EGroundMovementMode PreferredMovementMode = EGroundMovementMode::Trotting;

	UPROPERTY() UCameraComponent* ActiveCamera;
	UPROPERTY() UFlightHandler* FlightHandler;
	UPROPERTY() URotationInputHandler* RotationInputHandler;
	UPROPERTY(BlueprintReadOnly)
	UAnimationDebugReporter* AnimationDebugReporter;
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnLegPlanted OnLegPlanted;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* DragonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPhysicalAnimationComponent* PhysicalAnimation;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* DragonMeshAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* MeshRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* DetachedMeshRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* MainCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* AimCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Input callbacks
	void StartAimDownSights();
	void StopAimDownSights();

	void GroundMovement(const FInputActionValue& Value);
	void FlightCameraMove(const FInputActionValue& Value);
	void StartJump();
	void CancelFlight();
	void StartSprint();
	void StopSprint();
	void TogglePreferredGroundMovement();
	void CastSomeSpell();
	void SwitchGroundMovementMode(EGroundMovementMode NewMode);

public:
	virtual void PostInitializeComponents() override;
	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
