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
#include "Camera/CameraComponent.h"
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

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(GetRootComponent());
    SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 190.0f));
    SpringArm->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
    SpringArm->TargetArmLength = 1200.0f;
    
    MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
    MainCamera->SetupAttachment(SpringArm);

    AimCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("AimCamera"));
    AimCamera->SetupAttachment(DetachedMeshRoot);
    AimCamera->SetRelativeLocation(FVector(-40.0f, 240.0f, 864.0f));
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

void AMainCharacter::CalcCamera(const float DeltaTime, FMinimalViewInfo& OutResult)
{
    if (!ActiveCamera)
    {
        Super::CalcCamera(DeltaTime, OutResult);
        return;
    }
    
    TInlineComponentArray<UCameraComponent*> Cameras;
    GetComponents(/*out*/ Cameras);
    
    for (UCameraComponent* CameraComponent : Cameras)
    {
        if (CameraComponent->GetName() == ActiveCamera->GetName())
        {
            CameraComponent->GetCameraView(DeltaTime, OutResult);
            return;
        }
    }
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

    DesiredFacingRotation = GetActorRotation();
    RotationInputHandler->ResetRotation(GetActorRotation().Quaternion());

    UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
    if (!Input) return;
    
    Input->BindAction(UActions::Flight_FlapForward(), ETriggerEvent::Started, FlightHandler, &UFlightHandler::HandleFlapForwardInput);
    Input->BindAction(UActions::Flight_FlapForward(), ETriggerEvent::Completed, FlightHandler, &UFlightHandler::HandleFlapForwardInput);
    Input->BindAction(UActions::Flight_FlapForward(), ETriggerEvent::Canceled, FlightHandler, &UFlightHandler::HandleFlapForwardInput);
    Input->BindAction(UActions::Flight_FoldWings(), ETriggerEvent::Started, FlightHandler, &UFlightHandler::HandleFoldWingsInput);
    Input->BindAction(UActions::Flight_FoldWings(), ETriggerEvent::Completed, FlightHandler, &UFlightHandler::HandleFoldWingsInput);
    Input->BindAction(UActions::Flight_FoldWings(), ETriggerEvent::Canceled, FlightHandler, &UFlightHandler::HandleFoldWingsInput);
}

void AMainCharacter::StartAimDownSights()
{
    ActiveCamera = AimCamera;
    // APlayerController* PC = Cast<APlayerController>(GetController());
    MainCamera->Deactivate();
    AimCamera->Activate();
    // PC->SetViewTargetWithBlend(this, 0.5f, VTBlend_Cubic, 0.0f, false);
}

void AMainCharacter::StopAimDownSights()
{
    ActiveCamera = MainCamera;
    // APlayerController* PC = Cast<APlayerController>(GetController());
    MainCamera->Activate();
    AimCamera->Deactivate();
    // PC->SetViewTargetWithBlend(this, 0.5f, VTBlend_Cubic, 0.0f, false);
}

// Called every frame
void AMainCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

    const auto SpringArmComponent = FindComponentByClass<USpringArmComponent>();
    if (!Controller || !SpringArmComponent) { return; }

    const auto DesiredRotation = RotationInputHandler->GetCameraWorldRotation();

    if (FlightHandler->IsFlying())
    {
        const FRotator TargetRotationTwo = FRotator(0, 0, 0);
        SpringArmComponent->SetRelativeRotation(FMath::RInterpTo(SpringArmComponent->GetRelativeRotation(), TargetRotationTwo, GetWorld()->GetDeltaSeconds(), 5.0f));

        const auto TargetControlRotation = FRotator(0, DesiredRotation.Yaw, 0);
        Controller->SetControlRotation(FMath::RInterpTo(Controller->GetControlRotation(), TargetControlRotation, DeltaTime, 50.0f));

        const FQuat  CurrentQ = DetachedMeshRoot->GetComponentQuat();
        // const FQuat  TargetQ  = UE::Math::TQuat<double>::Slerp(GetCharacterMovement()->Velocity.ToOrientationQuat(), DesiredRotation.Quaternion(), 0.2f);
        const FQuat TargetQ = DesiredRotation.Quaternion();
        const FQuat  NewQ = FMath::QInterpTo(CurrentQ, TargetQ, DeltaTime, 10.f);
        DetachedMeshRoot->SetWorldRotation(NewQ);
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

            const FRotator NewDesiredRotation = RotationInputHandler->GetInputForwardWorldRotation();
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

        if (GetCharacterMovement()->IsMovingOnGround() && JumpCooldown > 0.0f)
        {
            JumpCooldown = FMath::Max(0.0f, JumpCooldown - DeltaTime);
        }
    }
    SpringArmComponent->SetWorldRotation(FRotator(DesiredRotation.Pitch, DesiredRotation.Yaw, DesiredRotation.Roll));
    SpringArmComponent->SetRelativeLocation(DetachedMeshRoot->GetUpVector() * FVector(0, 0, 190.0f));
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (!Input) return;

    Input->BindAction(UActions::HoldAimDownSights(), ETriggerEvent::Started, this, &AMainCharacter::StartAimDownSights);
    Input->BindAction(UActions::HoldAimDownSights(), ETriggerEvent::Completed, this, &AMainCharacter::StopAimDownSights);
    Input->BindAction(UActions::HoldAimDownSights(), ETriggerEvent::Canceled, this, &AMainCharacter::StopAimDownSights);
    
    Input->BindAction(UActions::GroundMovement(), ETriggerEvent::Triggered, this, &AMainCharacter::GroundMovement);
    
    Input->BindAction(UActions::GroundMovement(), ETriggerEvent::Triggered, RotationInputHandler, &URotationInputHandler::HandleMovementInput);
    Input->BindAction(UActions::CameraMove(), ETriggerEvent::Triggered, RotationInputHandler, &URotationInputHandler::HandleRotationInput);
    Input->BindAction(UActions::Flight_Roll(), ETriggerEvent::Triggered, RotationInputHandler, &URotationInputHandler::HandleFlightRollInput);
    
    Input->BindAction(UActions::Jump(), ETriggerEvent::Started, this, &AMainCharacter::StartJump);
    Input->BindAction(UActions::Flight_Cancel(), ETriggerEvent::Started, this, &AMainCharacter::CancelFlight);

    Input->BindAction(UActions::Sprint(), ETriggerEvent::Started, this, &AMainCharacter::StartSprint);
    Input->BindAction(UActions::Sprint(), ETriggerEvent::Completed, this, &AMainCharacter::StopSprint);
    Input->BindAction(UActions::Sprint(), ETriggerEvent::Canceled, this, &AMainCharacter::StopSprint);

    Input->BindAction(UActions::TogglePreferredMovement(), ETriggerEvent::Started, this, &AMainCharacter::TogglePreferredGroundMovement);
    
    Input->BindAction(UActions::CastASpell(), ETriggerEvent::Triggered, this, &AMainCharacter::CastSomeSpell);
}

FRotator AMainCharacter::GetMeshActorRotation() const
{
    return DetachedMeshRoot->GetComponentRotation();
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
        if (JumpCooldown > 0.0f)
            return;
        
        const float JumpZVelocity = GetCharacterMovement()->JumpZVelocity;
        const FVector LaunchDirection = FVector(0, 0, JumpZVelocity);
        const FVector ForwardStrength = GetCharacterMovement()->GetForwardVector() * FVector(250.0f, 250.0f, 0) * FMath::Clamp(GetVelocity().Size() / 600.0f, 0.0f, 1.0f);
        LaunchCharacter(LaunchDirection + ForwardStrength, false, true);
        const auto AnimInstance = Cast<UDragonAnimInstance>(DragonMesh->GetAnimInstance());
        AnimInstance->StateMachine->SetState(EAnimationState::Jumping);
        AnimInstance->StateMachine->AnimationLockout = 0.2f;
        JumpCooldown = 0.10f;
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
    GetCharacterMovement()->SetMovementMode(MOVE_Falling);
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
