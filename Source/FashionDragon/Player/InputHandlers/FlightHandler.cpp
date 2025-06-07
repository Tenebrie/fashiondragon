#include "FlightHandler.h"

#include "RotationInputHandler.h"
#include "FashionDragon/DebugTools/QuickDebug.h"
#include "FashionDragon/Player/MainCharacter.h"
#include "FashionDragon/Player/Animation/DragonAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

void UFlightHandler::BeginPlay()
{
	Super::BeginPlay();
	LeftWing = GetAnimInstance()->LeftWing.GetPostProcessBone();
	RightWing = GetAnimInstance()->RightWing.GetPostProcessBone();
}

void UFlightHandler::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const auto Character = GetCharacter();
	if (!IsInFlight || !Character || !Character->GetMovementComponent()) { return; }

	const auto MovementComponent = GetCharacter()->GetCharacterMovement();

	// const auto FlapHappened = (LeftWing->Flap - LastFrameLeftFlap) * LeftWing->Openness + (RightWing->Flap - LastFrameRightFlap) * RightWing->Openness;

	// const FVector LiftVector = FVector(0.0f, 0.0f, FlapHappened * 900.0f);

	// MovementComponent->AddImpulse(LiftVector, true);

	// LastFrameLeftFlap = LeftWing->Flap;
	// LastFrameRightFlap = RightWing->Flap;
	// LastFrameLeftOpenness = LeftWing->Openness;
	// LastFrameRightOpenness = RightWing->Openness;

	const FRotator TargetRotation = GetCharacter()->RotationInputHandler->GetRotation();

	// ─── Tunables you’ll actually tweak in-editor ──────────────────────
	const float IdealGlideSpeed    = 2500.f;   // speed where wings work best
	const float LiftGravityRatio   = 0.85f;    // at ideal speed, lift = 0.85 g  (<1 ⇒ slow sink)
	const float DragCoeff          = 0.00002f;   // quadratic drag; tiny keeps momentum
	const float BankCoeff          = 2.0f;     // turn bite (higher = twitchy)
	const float EngineAccel        = 2200.f;   // mild magic/thrust (won’t beat gravity straight up)
	const float StallSpeed         = 300.f;    // below this you basically free-fall
	// ───────────────────────────────────────────────────────────────────

	const FVector Gravity(0.f, 0.f, GetWorld()->GetGravityZ());     // (0,0,-980)
	const float   GravityMag = -Gravity.Z;

	const FVector DesiredDir = TargetRotation.Vector().GetSafeNormal();
	FVector       Vel        = MovementComponent->Velocity;
	float         Speed      = Vel.Size();
	FVector       VelDir     = Speed > KINDA_SMALL_NUMBER ? Vel / Speed : DesiredDir;

	const float Dt = GetWorld()->GetDeltaSeconds();

	/* 1. Engine thrust (small; you gain real speed by diving) */
	const FVector ThrustAccel = DesiredDir * EngineAccel;

	/* 2. Quadratic drag */
	const FVector DragAccel   = -VelDir * Speed * Speed * DragCoeff;

	/* 3. Lift — capped so it never fully beats gravity
	 *    – scales with horizontal component of flight
	 *    – scales with speed, topping out near IdealGlideSpeed            */
	FVector LiftDir = FVector::CrossProduct(VelDir, FVector::CrossProduct(FVector::UpVector, VelDir)).GetSafeNormal();
	float   horizFactor = 1.f - FMath::Abs(VelDir.Z);                         // 1 when level, 0 when vertical
	float   speedFactor = FMath::Clamp(Speed / IdealGlideSpeed, 0.f, 1.f);    // 0->1
	float   liftMag     = GravityMag * LiftGravityRatio * horizFactor * speedFactor;
	FVector LiftAccel   = LiftDir * liftMag;

	/* 4. Bank / steering force — pulls velocity toward controller heading */
	FVector BankAccel = (DesiredDir - VelDir) * (Speed * BankCoeff);

	/* 5. Total acceleration + integrate */
	FVector Accel      = Gravity + ThrustAccel + DragAccel + LiftAccel + BankAccel;
	FVector NewVelocity = Vel + Accel * Dt;

	/* 6. Stall: if too slow, kill lift & bank so you dive and rebuild speed */
	if (NewVelocity.Size() < StallSpeed)
	{
	    NewVelocity += Gravity * Dt;     // accelerate downward
	}

	MovementComponent->Velocity = NewVelocity;
}

void UFlightHandler::StartFlight()
{
	IsInFlight = true;
}

