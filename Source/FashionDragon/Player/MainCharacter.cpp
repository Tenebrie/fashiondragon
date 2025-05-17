// Fill out your copyright notice in the Description page of Project Settings.
#include "MainCharacter.h"

#include "Animation/DragonAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "EnhancedInputComponent.h"
#include "FashionDragon/DebugTools/QuickDebug.h"
#include "Input/Actions.h"

#include <iostream>
#include <vector>
#include <string>
#include <chrono>

// Sets default values
AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(50.f, 50.0f);
    
    MeshRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DragonMeshRoot"));
    MeshRoot->SetupAttachment(GetCapsuleComponent());
    MeshRoot->SetRelativeLocation(FVector(0.f, 0.f, 85.f));

    DragonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DragonMesh"));
    DragonMesh->SetupAttachment(MeshRoot);
    DragonMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
    DragonMesh->SetSkeletalMeshAsset(DragonMeshAsset);

    PhysicalAnimation = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnimationComponent"));
    PhysicalAnimation->SetSkeletalMeshComponent(DragonMesh);
}

void AMainCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
    PhysicalAnimation->ApplyPhysicalAnimationProfileBelow(TEXT("Tail_001"), TEXT("MyProfile"), true);
    PhysicalAnimation->ApplyPhysicalAnimationProfileBelow(TEXT("Wing_001_R"), TEXT("MyProfile"), true);
    PhysicalAnimation->ApplyPhysicalAnimationProfileBelow(TEXT("Wing_001_L"), TEXT("MyProfile"), true);

    DragonMesh->SetConstraintProfileForAll(TEXT("MyProfile"));
    DragonMesh->SetAllBodiesBelowSimulatePhysics(TEXT("Tail_001"), true, true);
    DragonMesh->SetAllBodiesBelowSimulatePhysics(TEXT("Wing_001_R"), true, true);
    DragonMesh->SetAllBodiesBelowSimulatePhysics(TEXT("Wing_001_L"), true, true);

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;
    GetCharacterMovement()->MaxWalkSpeed = 800.f;
    GetCharacterMovement()->MaxAcceleration = 1000.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
}

// Called every frame
void AMainCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
    if (!Input) return;
    
    Input->BindAction(UActions::GroundMovement, ETriggerEvent::Triggered, this, &AMainCharacter::GroundMovement);
    Input->BindAction(UActions::CameraMove, ETriggerEvent::Triggered, this, &AMainCharacter::CameraMove);
    
    Input->BindAction(UActions::Jump, ETriggerEvent::Started, this, &AMainCharacter::StartJump);
    Input->BindAction(UActions::Jump, ETriggerEvent::Completed & ETriggerEvent::Canceled, this, &AMainCharacter::ReleaseJump);

    Input->BindAction(UActions::Sprint, ETriggerEvent::Started, this, &AMainCharacter::StartSprint);
    Input->BindAction(UActions::Sprint, ETriggerEvent::Completed & ETriggerEvent::Canceled, this, &AMainCharacter::StopSprint);
    
    Input->BindAction(UActions::CastASpell, ETriggerEvent::Started, this, &AMainCharacter::CastSomeSpell);
}

void AMainCharacter::GroundMovement(const FInputActionValue& Value)
{
    if (!Controller) { return; }

    const FVector2D MovementVector = Value.Get<FVector2D>();
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRot(0, Rotation.Yaw, 0);
    const FVector ForwardDirection = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
    AddMovementInput(ForwardDirection, MovementVector.Y);
    AddMovementInput(RightDirection, MovementVector.X);
}

void AMainCharacter::CameraMove(const FInputActionValue& Value)
{
    const auto SpringArmComponent = FindComponentByClass<USpringArmComponent>();
    if (!Controller || !SpringArmComponent) { return; }

    const FVector2D RotationDelta = Value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(RotationDelta.X))
    {
        AddControllerYawInput(RotationDelta.X * 0.5f);
    }

    if (!FMath::IsNearlyZero(RotationDelta.Y))
    {
        FRotator NewRotation = SpringArmComponent->GetRelativeRotation();
        NewRotation.Pitch += RotationDelta.Y;
        NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, -80.f, 80.f);
        SpringArmComponent->SetRelativeRotation(NewRotation);
    }
}

void AMainCharacter::StartJump()
{
    if (GetCharacterMovement()->IsMovingOnGround())
    {
        const float JumpZVelocity = GetCharacterMovement()->JumpZVelocity;
        LaunchCharacter(FVector(0, 0, JumpZVelocity), false, true);
        const auto AnimInstance = Cast<UDragonAnimInstance>(DragonMesh->GetAnimInstance());
        AnimInstance->StateMachine->SetState(Jumping);
        AnimInstance->StateMachine->AnimationLockout = 0.5f;
    }
}

void AMainCharacter::ReleaseJump()
{
}

void AMainCharacter::StartSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = 2500.f;
    IsSprinting = true;
}

void AMainCharacter::StopSprint()
{
    GetCharacterMovement()->MaxWalkSpeed = 800.f;
    IsSprinting = false;
}

void AMainCharacter::CastSomeSpell()
{
    Debug::Print("We are casting a speeeeeell~");
}
