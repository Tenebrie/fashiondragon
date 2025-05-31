// Fill out your copyright notice in the Description page of Project Settings.
// ReSharper disable CppMemberFunctionMayBeStatic
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
#include "InputActions/Actions.h"
#include "InputActions/DefaultPlayerController.h"
#include "InputHandlers/FlightHandler.h"
#include "InputHandlers/RotationInputHandler.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(50.f, 50.0f);

    DetachedMeshRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RotationRoot"));
    DetachedMeshRoot->SetupAttachment(GetCapsuleComponent());
    
    MeshRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DragonMeshRoot"));
    MeshRoot->SetupAttachment(DetachedMeshRoot);
    MeshRoot->SetRelativeLocation(FVector(0.f, 0.f, 85.f));

    DragonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DragonMesh"));
    DragonMesh->SetupAttachment(MeshRoot);
    DragonMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
    DragonMesh->SetSkeletalMeshAsset(DragonMeshAsset);

    PhysicalAnimation = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnimationComponent"));
    PhysicalAnimation->SetSkeletalMeshComponent(DragonMesh);
    DetachedMeshRoot->SetUsingAbsoluteRotation(true);
}

void AMainCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    FlightHandler = NewObject<UFlightHandler>(this);
    RotationInputHandler = NewObject<URotationInputHandler>(this);
    FlightHandler->RegisterComponent();
    RotationInputHandler->RegisterComponent();
    AddInstanceComponent(FlightHandler);
    AddInstanceComponent(RotationInputHandler);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
    PhysicalAnimation->ApplyPhysicalAnimationProfileBelow(TEXT("Tail_001"), TEXT("MyProfile"), true);
    PhysicalAnimation->ApplyPhysicalAnimationProfileBelow(TEXT("Back_R"), TEXT("MyProfile"), true);
    PhysicalAnimation->ApplyPhysicalAnimationProfileBelow(TEXT("Back_L"), TEXT("MyProfile"), true);

    DragonMesh->SetConstraintProfileForAll(TEXT("MyProfile"));
    DragonMesh->SetAllBodiesBelowSimulatePhysics(TEXT("Tail_001"), true, true);
    DragonMesh->SetAllBodiesBelowSimulatePhysics(TEXT("Back_R"), true, true);
    DragonMesh->SetAllBodiesBelowSimulatePhysics(TEXT("Back_L"), true, true);

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;
    GetCharacterMovement()->MaxWalkSpeed = 2000.f;
    GetCharacterMovement()->MaxAcceleration = 2048.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
    // MeshRoot->SetUsingAbsoluteRotation(true);

    SwitchGroundMovementMode(EGroundMovementMode::Trotting);
}

// Called every frame
void AMainCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

    const auto SpringArmComponent = FindComponentByClass<USpringArmComponent>();
    if (!Controller || !SpringArmComponent) { return; }

    // TODO: Detach mesh movement from camera movement
    const FRotator CameraRotation = SpringArmComponent->GetComponentRotation();
    const FRotator TargetRotation = FRotator(0.f, CameraRotation.Yaw - 90, 0.f);
    const float CurrentYaw = MeshRoot->GetComponentRotation().Yaw;
    const float TargetYaw = TargetRotation.Yaw;
    const float NewYaw = FMath::FixedTurn(CurrentYaw, TargetYaw, DeltaTime * 5000.0f);

    const FRotator NewRot = FRotator(0.f, NewYaw, 0.f);
    MeshRoot->SetWorldRotation(NewRot);

    // Return camera to default in flight
    if (FlightHandler->IsFlying())
    {
        const FRotator TargetRotationTwo = FRotator(0, 0, 0);
        SpringArmComponent->SetRelativeRotation(FMath::RInterpTo(SpringArmComponent->GetRelativeRotation(), TargetRotationTwo, GetWorld()->GetDeltaSeconds(), 5.0f));
    }

    const auto DesiredRotation = RotationInputHandler->GetRotation();
    // Print debug line forward, using desired rotation (global space)
    const FVector ForwardVector = DesiredRotation.RotateVector(FVector::ForwardVector);
    const FVector StartLocation = MeshRoot->GetComponentLocation();
    const FVector EndLocation = StartLocation + ForwardVector * 1000.0f;
    DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor(255, 0, 255), 0.f, 5.f);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
    if (!Input) return;
    
    Input->BindAction(UActions::GroundMovement(), ETriggerEvent::Triggered, this, &AMainCharacter::GroundMovement);
    Input->BindAction(UActions::CameraMove(), ETriggerEvent::Triggered, this, &AMainCharacter::CameraMove);
    Input->BindAction(UActions::FlightCamera(), ETriggerEvent::Triggered, this, &AMainCharacter::FlightCameraMove);
    Input->BindAction(UActions::CameraMove(), ETriggerEvent::Triggered, RotationInputHandler, &URotationInputHandler::HandleInput);
    
    Input->BindAction(UActions::Jump(), ETriggerEvent::Started, this, &AMainCharacter::StartJump);
    Input->BindAction(UActions::Jump(), ETriggerEvent::Completed & ETriggerEvent::Canceled, this, &AMainCharacter::ReleaseJump);

    Input->BindAction(UActions::Sprint(), ETriggerEvent::Started, this, &AMainCharacter::StartSprint);
    Input->BindAction(UActions::Sprint(), ETriggerEvent::Completed & ETriggerEvent::Canceled, this, &AMainCharacter::StopSprint);

    Input->BindAction(UActions::TogglePreferredMovement(), ETriggerEvent::Started, this, &AMainCharacter::TogglePreferredGroundMovement);
    
    Input->BindAction(UActions::CastASpell(), ETriggerEvent::Triggered, this, &AMainCharacter::CastSomeSpell);
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

void AMainCharacter::FlightCameraMove(const FInputActionValue& Value)
{
    const FVector2D Delta = Value.Get<FVector2D>();

    const auto DeltaRot = FRotator(Delta.Y, 0, 0.f);

    AddActorWorldRotation(DeltaRot);
    AddControllerYawInput(Delta.X * 0.5f);
}

void AMainCharacter::StartJump()
{
    if (GetCharacterMovement()->IsMovingOnGround())
    {
        const float JumpZVelocity = GetCharacterMovement()->JumpZVelocity;
        LaunchCharacter(FVector(0, 0, JumpZVelocity), false, true);
        const auto AnimInstance = Cast<UDragonAnimInstance>(DragonMesh->GetAnimInstance());
        AnimInstance->StateMachine->SetState(EAnimationState::Jumping);
        AnimInstance->StateMachine->AnimationLockout = 0.2f;
    }
    else
    {
        // FlightController->StartFlight();
        // const auto PlayerController = Cast<ADefaultPlayerController>(GetController());
        // PlayerController->SetControlMode(EControlMode::Flying);

        // GetCharacterMovement()->AirControl = 1.0f;
        // GetCharacterMovement()->MaxWalkSpeed = 0.f;
    }
}

void AMainCharacter::ReleaseJump()
{
}

void AMainCharacter::StartSprint() { SwitchGroundMovementMode(EGroundMovementMode::Sprinting); }
void AMainCharacter::StopSprint() { SwitchGroundMovementMode(PreferredMovementMode); }

void AMainCharacter::TogglePreferredGroundMovement()
{
    if (PreferredMovementMode == EGroundMovementMode::Walking)
        PreferredMovementMode = EGroundMovementMode::Trotting;
    else
        PreferredMovementMode = EGroundMovementMode::Walking;

    if (MovementMode != EGroundMovementMode::Sprinting)
        SwitchGroundMovementMode(PreferredMovementMode);
}

void AMainCharacter::CastSomeSpell()
{
}

void AMainCharacter::SwitchGroundMovementMode(const EGroundMovementMode NewMode)
{
    auto WalkSpeed = 600.f;
    switch (NewMode)
    {
    case EGroundMovementMode::Walking:
        WalkSpeed = 600.0f;
        break;
    case EGroundMovementMode::Trotting:
        WalkSpeed = 1400.0f;
        break;
    case EGroundMovementMode::Sprinting:
        WalkSpeed = 2400.0f;
        break;
    }

    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    MovementMode = NewMode;
}
