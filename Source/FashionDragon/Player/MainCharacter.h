// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAnimationDebugReporter;
class UFlightHandler;
class URotationInputHandler;
class UDragonFlightHandler;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLegPlanted, FVector, SpawnLocation);

UENUM(BlueprintType)
enum class EGroundMovementMode : uint8
{
	Walking,
	Trotting,
	Sprinting,
};

UCLASS(BlueprintType)
class FASHIONDRAGON_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

	UPROPERTY(BlueprintReadWrite) FRotator DesiredFacingRotation = FRotator::ZeroRotator;

	UPROPERTY(BlueprintReadWrite) EGroundMovementMode MovementMode;
	UPROPERTY(BlueprintReadWrite) EGroundMovementMode PreferredMovementMode = EGroundMovementMode::Trotting;

	UPROPERTY(BlueprintReadOnly) UCameraComponent* ActiveCamera;
	UPROPERTY(BlueprintReadOnly) UFlightHandler* FlightHandler;
	UPROPERTY(BlueprintReadOnly) URotationInputHandler* RotationInputHandler;
	UPROPERTY(BlueprintReadOnly) UAnimationDebugReporter* AnimationDebugReporter;
	
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

	float JumpCooldown = 0.0f;

protected:
	// Called when the game starts or when spawned
	UFUNCTION(BlueprintCallable) virtual void BeginPlay() override;

	// Input callbacks
	UFUNCTION(BlueprintCallable) void StartAimDownSights();
	UFUNCTION(BlueprintCallable) void StopAimDownSights();

	UFUNCTION(BlueprintCallable) void GroundMovement(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable) void FlightCameraMove(const FInputActionValue& Value);
	UFUNCTION(BlueprintCallable) void StartJump();
	UFUNCTION(BlueprintCallable) void CancelFlight();
	UFUNCTION(BlueprintCallable) void StartSprint();
	UFUNCTION(BlueprintCallable) void StopSprint();
	UFUNCTION(BlueprintCallable) void TogglePreferredGroundMovement();
	UFUNCTION(BlueprintCallable) void CastSomeSpell();
	UFUNCTION(BlueprintCallable) void SwitchGroundMovementMode(EGroundMovementMode NewMode);

public:
	virtual void PostInitializeComponents() override;
	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	FRotator GetMeshActorRotation() const;
	FVector GetMeshActorScale() const;
};
