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
#include "Animation/Debug/AnimationDebugReporter.h"
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
    AnimationDebugReporter = NewObject<UAnimationDebugReporter>(this);
    FlightHandler->RegisterComponent();
    RotationInputHandler->RegisterComponent();
    AnimationDebugReporter->RegisterComponent();
    AddInstanceComponent(FlightHandler);
    AddInstanceComponent(RotationInputHandler);
    AddInstanceComponent(AnimationDebugReporter);
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
    
    PhysicalAnimation->SetSkeletalMeshComponent(DragonMesh);
    PhysicalAnimation->ApplyPhysicalAnimationProfileBelow(TEXT("Back_L"), TEXT("MyProfile"), true);
    PhysicalAnimation->ApplyPhysicalAnimationProfileBelow(TEXT("Back_R"), TEXT("MyProfile"), true);
    DragonMesh->SetAllBodiesBelowPhysicsBlendWeight(TEXT("Back_R"), 1.0f, true, true);
    DragonMesh->SetAllBodiesBelowPhysicsBlendWeight(TEXT("Back_L"), 1.0f, true, true);
    DragonMesh->SetAllBodiesBelowSimulatePhysics(TEXT("Tail_001"), true, true);
    DragonMesh->SetAllBodiesBelowSimulatePhysics(TEXT("Back_R"), true, true);
    DragonMesh->SetAllBodiesBelowSimulatePhysics(TEXT("Back_L"), true, true);

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 0.75f;
    GetCharacterMovement()->MaxAcceleration = 4096.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
    DetachedMeshRoot->SetUsingAbsoluteRotation(true);

    SwitchGroundMovementMode(EGroundMovementMode::Trotting);

    RotationInputHandler->ResetRotation(GetActorRotation().Quaternion());
}

// Called every frame
void AMainCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

    const auto SpringArmComponent = FindComponentByClass<USpringArmComponent>();
    if (!Controller || !SpringArmComponent) { return; }

    const auto DesiredRotation = RotationInputHandler->GetCameraWorldRotation();

    // Return camera to default in flight
    if (FlightHandler->IsFlying())
    {
        const FRotator TargetRotationTwo = FRotator(0, 0, 0);
        SpringArmComponent->SetRelativeRotation(FMath::RInterpTo(SpringArmComponent->GetRelativeRotation(), TargetRotationTwo, GetWorld()->GetDeltaSeconds(), 5.0f));

        const auto TargetControlRotation = FRotator(0, DesiredRotation.Yaw, 0);
        Controller->SetControlRotation(FMath::RInterpTo(Controller->GetControlRotation(), TargetControlRotation, DeltaTime, 50.0f));

        const auto TargetRot = FRotator(-DesiredRotation.Roll, 0, -DesiredRotation.Pitch);
        MeshRoot->SetRelativeRotation(TargetRot);
    }
    else
    {
        const auto TargetControlRotation = FRotator(0, DesiredRotation.Yaw, 0);
        SpringArmComponent->SetWorldRotation(FRotator(DesiredRotation.Pitch, DesiredRotation.Yaw, 0.0f));

        if (GetMovementComponent()->Velocity.Size() > KINDA_SMALL_NUMBER)
        {
            Controller->SetControlRotation(FMath::RInterpTo(Controller->GetControlRotation(), TargetControlRotation, DeltaTime, 50.0f));

            const auto MovementSpeedScalar = FMath::Clamp(GetMovementComponent()->Velocity.Size() / 700.0f, 0, 1);
            const auto CurrentRot = DetachedMeshRoot->GetComponentRotation();

            const FRotator NewDesiredRotation = RotationInputHandler->GetFacingWorldRotation();
            if (NewDesiredRotation.Vector().Dot(DesiredFacingRotation.Vector()) < -0.6f)
            {
                DesiredFacingRotation = NewDesiredRotation + FRotator(0.0f, 180.0f,0.0f);
            }
            else
            {
                DesiredFacingRotation = NewDesiredRotation;
            }

            const FRotator FacingRotation = DesiredFacingRotation;
            const auto NewRot = FMath::RInterpTo(CurrentRot, FacingRotation, DeltaTime, 10.0f * MovementSpeedScalar);
            DetachedMeshRoot->SetWorldRotation(NewRot);
        }
    }
    SpringArmComponent->SetWorldRotation(FRotator(DesiredRotation.Pitch, DesiredRotation.Yaw, 0.0f));
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (!Input) return;
    
    Input->BindAction(UActions::GroundMovement(), ETriggerEvent::Triggered, this, &AMainCharacter::GroundMovement);
    
    Input->BindAction(UActions::GroundMovement(), ETriggerEvent::Triggered, RotationInputHandler, &URotationInputHandler::HandleMovementInput);
    Input->BindAction(UActions::CameraMove(), ETriggerEvent::Triggered, RotationInputHandler, &URotationInputHandler::HandleRotationInput);
    
    Input->BindAction(UActions::Jump(), ETriggerEvent::Started, this, &AMainCharacter::StartJump);
    Input->BindAction(UActions::CancelFlight(), ETriggerEvent::Started, this, &AMainCharacter::CancelFlight);

    Input->BindAction(UActions::Sprint(), ETriggerEvent::Started, this, &AMainCharacter::StartSprint);
    Input->BindAction(UActions::Sprint(), ETriggerEvent::Completed | ETriggerEvent::Canceled, this, &AMainCharacter::StopSprint);

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
        const FVector LaunchDirection = FVector(0, 0, JumpZVelocity);
        const FVector ForwardStrength = GetCharacterMovement()->GetForwardVector() * FVector(250.0f, 250.0f, 0) * FMath::Clamp(GetVelocity().Size() / 600.0f, 0.0f, 1.0f);
        LaunchCharacter(LaunchDirection + ForwardStrength, false, true);
        const auto AnimInstance = Cast<UDragonAnimInstance>(DragonMesh->GetAnimInstance());
        AnimInstance->StateMachine->SetState(EAnimationState::Jumping);
        AnimInstance->StateMachine->AnimationLockout = 0.2f;
    }
    else
    {
        FlightHandler->StartFlight();
        const auto PlayerController = Cast<ADefaultPlayerController>(GetController());
        PlayerController->SetControlMode(EControlMode::Flying);
        GetCharacterMovement()->SetMovementMode(MOVE_Flying);
    }
}

void AMainCharacter::CancelFlight()
{
    FlightHandler->CancelFlight();
    const auto PlayerController = Cast<ADefaultPlayerController>(GetController());
    PlayerController->SetControlMode(EControlMode::Ground);
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
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
