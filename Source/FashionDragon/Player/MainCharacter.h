// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "MainCharacter.generated.h"

UCLASS()
class FASHIONDRAGON_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	bool IsSprinting;
	bool IsChargingJump;
	float JumpCharge = 0.0f;

private:
	UPROPERTY(EditAnywhere, Category = "Assets", meta = (AllowPrivateAccess = "true"))
	USkeletalMesh* DragonMeshAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* MeshRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* DragonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UPhysicalAnimationComponent* PhysicalAnimation;

	bool IsMovingBackward() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Input callbacks
	void MoveForward(float Value);
	void MoveRight(float Value);
	void StartJump();
	void ReleaseJump();
	void StartSprint();
	void StopSprint();

	void LookHorizontal(float Value);
	void LookVertical(float Value);

	void UpdateBoneTransforms();

public:
	virtual void PostInitializeComponents() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
