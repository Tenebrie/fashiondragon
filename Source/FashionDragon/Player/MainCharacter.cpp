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

    PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookHorizontal", this, &AMainCharacter::LookHorizontal);
    PlayerInputComponent->BindAxis("LookVertical", this, &AMainCharacter::LookVertical);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::ReleaseJump);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::StopSprint);
}

void AMainCharacter::MoveForward(const float Value)
{
    if (Controller && Value != 0.0f && !IsChargingJump)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRot(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void AMainCharacter::MoveRight(const float Value)
{
    if (Controller && Value != 0.0f && !IsChargingJump)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRot(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
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

void AMainCharacter::LookHorizontal(const float Value)
{
    if (!Controller || Value == 0.0f) return;

    FRotator NewRotation = Controller->GetControlRotation();
    NewRotation.Yaw += Value;
    Controller->SetControlRotation(NewRotation);
}

void AMainCharacter::LookVertical(const float Value)
{
    if (!Controller || Value == 0.0f) return;

    // Rotate the camera up and down
    if (const auto SpringArmComponent = FindComponentByClass<USpringArmComponent>())
    {
        FRotator NewRotation = SpringArmComponent->GetRelativeRotation();
        NewRotation.Pitch += Value;
        NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, -80.f, 80.f);
        SpringArmComponent->SetRelativeRotation(NewRotation);
    }
}
