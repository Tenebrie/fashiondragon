// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "DragonAnimInstance.h"

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

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
    GetCharacterMovement()->MaxWalkSpeed = 200.f;
    GetCharacterMovement()->MaxAcceleration = 50.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 40.f;

    DragonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DragonMesh"));
    DragonMesh->SetupAttachment(GetCapsuleComponent());
    
    DragonMesh->SetSkeletalMeshAsset(DragonMeshAsset);
    
    // Create the skeletal mesh component
    // SkeletalMeshComponent->SetEnableGravity(true);
    // SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    // SkeletalMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
    // SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

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
}

// Called every frame
void AMainCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

    const auto AnimInstance = Cast<UDragonAnimInstance>(DragonMesh->GetAnimInstance());

    const auto CurrentSpeed = GetVelocity().Size();
    const auto SpeedMultiplier = IsMovingBackward() ? -1.0f : 1.0f;

    const auto AdvanceValue = DeltaTime * 0.0035f * CurrentSpeed * SpeedMultiplier;
    AnimInstance->Advance(AdvanceValue, DeltaTime);

    // const auto Location = DragonMesh->GetBoneLocation(TEXT("Tail_001"), EBoneSpaces::WorldSpace);
    // UE_LOG(LogTemp, Display, TEXT("Location: %f"), Location.Z);
}

bool AMainCharacter::IsMovingBackward() const
{
    const FVector Velocity = GetVelocity();
    const FVector ForwardVector = GetActorForwardVector();
    return FVector::DotProduct(Velocity.GetSafeNormal(), ForwardVector) < 0.0f;
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
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::StopJump);
}

void AMainCharacter::MoveForward(const float Value)
{
    if (Controller && Value != 0.0f)
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRot(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void AMainCharacter::MoveRight(const float Value)
{
    if (Controller && Value != 0.0f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRot(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void AMainCharacter::StartJump()
{
    const float JumpZVelocity = GetCharacterMovement()->JumpZVelocity;
    // Add physical impulse up manually (without using Jump())
    if (GetCharacterMovement()->IsMovingOnGround())
    {
        LaunchCharacter(FVector(0, 0, JumpZVelocity), false, true);
    }
    else
    {
        LaunchCharacter(FVector(0, 0, JumpZVelocity * 0.5f), false, true);
    }
    
}

void AMainCharacter::StopJump()
{
    StopJumping();
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
