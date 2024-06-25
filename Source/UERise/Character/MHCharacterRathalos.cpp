// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MHCharacterRathalos.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/MHAIController.h"
#include "MonsterStat/MHRathalosStatComponent.h"
#include "Physics/MHCollision.h"
#include "Engine/DamageEvents.h"
#include "AI/MHAI.h"

// Sets default values
AMHCharacterRathalos::AMHCharacterRathalos()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_MHCAPSULE);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -99.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/ImportSource/RathalosSK/Rathalos4.Rathalos4"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Script/Engine.AnimBlueprint'/Game/Animations/ABP_MHCharacterRathalos.ABP_MHCharacterRathalos_C'"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// Stat
	Stat = CreateDefaultSubobject<UMHRathalosStatComponent>(TEXT("Stat"));

	// Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RotateLeft90MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_RotateLeft90.AM_RotateLeft90'"));
	if (RotateLeft90MontageRef.Object)
	{
		RotateLeft90Montage = RotateLeft90MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RotateLeft180MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_RotateLeft180.AM_RotateLeft180'"));
	if (RotateLeft180MontageRef.Object)
	{
		RotateLeft180Montage = RotateLeft180MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RotateRight90MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_RotateRight90.AM_RotateRight90'"));
	if (RotateRight90MontageRef.Object)
	{
		RotateRight90Montage = RotateRight90MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RotateRight180MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_RotateRight180.AM_RotateRight180'"));
	if (RotateRight180MontageRef.Object)
	{
		RotateRight180Montage = RotateRight180MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Rotate2Left90MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_Rotate2Left90.AM_Rotate2Left90'"));
	if (Rotate2Left90MontageRef.Object)
	{
		Rotate2Left90Montage = Rotate2Left90MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Rotate2Left180MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_Rotate2Left180.AM_Rotate2Left180'"));
	if (Rotate2Left180MontageRef.Object)
	{
		Rotate2Left180Montage = Rotate2Left180MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Rotate2Right90MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_Rotate2Right90.AM_Rotate2Right90'"));
	if (Rotate2Right90MontageRef.Object)
	{
		Rotate2Right90Montage = Rotate2Right90MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Rotate2Right180MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_Rotate2Right180.AM_Rotate2Right180'"));
	if (Rotate2Right180MontageRef.Object)
	{
		Rotate2Right180Montage = Rotate2Right180MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> NoDetectMoveForwardMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_NoDetectMoveForward.AM_NoDetectMoveForward'"));
	if (NoDetectMoveForwardMontageRef.Object)
	{
		NoDetectMoveForwardMontage = NoDetectMoveForwardMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DetectMoveForwardMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_DetectMoveForward.AM_DetectMoveForward'"));
	if (DetectMoveForwardMontageRef.Object)
	{
		DetectMoveForwardMontage = DetectMoveForwardMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RoarMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_Roar.AM_Roar'"));
	if (RoarMontageRef.Object)
	{
		RoarMontage = RoarMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackPattern1MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_AttackPattern1.AM_AttackPattern1'"));
	if (AttackPattern1MontageRef.Object)
	{
		AttackPattern1Montage = AttackPattern1MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackPattern2MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_AttackPattern2.AM_AttackPattern2'"));
	if (AttackPattern2MontageRef.Object)
	{
		AttackPattern2Montage = AttackPattern2MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackPattern3MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_AttackPattern3.AM_AttackPattern3'"));
	if (AttackPattern3MontageRef.Object)
	{
		AttackPattern3Montage = AttackPattern3MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackPattern4MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_AttackPattern4.AM_AttackPattern4'"));
	if (AttackPattern4MontageRef.Object)
	{
		AttackPattern4Montage = AttackPattern4MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackPattern5MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_AttackPattern5.AM_AttackPattern5'"));
	if (AttackPattern5MontageRef.Object)
	{
		AttackPattern5Montage = AttackPattern5MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackPattern5_1MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_AttackPattern5_1.AM_AttackPattern5_1'"));
	if (AttackPattern5_1MontageRef.Object)
	{
		AttackPattern5_1Montage = AttackPattern5_1MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackPattern5_2MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_AttackPattern5_2.AM_AttackPattern5_2'"));
	if (AttackPattern5_2MontageRef.Object)
	{
		AttackPattern5_2Montage = AttackPattern5_2MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackPattern6MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_AttackPattern6.AM_AttackPattern6'"));
	if (AttackPattern6MontageRef.Object)
	{
		AttackPattern6Montage = AttackPattern6MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackPattern7MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_AttackPattern7.AM_AttackPattern7'"));
	if (AttackPattern7MontageRef.Object)
	{
		AttackPattern7Montage = AttackPattern7MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackPattern8MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_AttackPattern8.AM_AttackPattern8'"));
	if (AttackPattern8MontageRef.Object)
	{
		AttackPattern8Montage = AttackPattern8MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackPattern8_1MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_AttackPattern8_1.AM_AttackPattern8_1'"));
	if (AttackPattern8_1MontageRef.Object)
	{
		AttackPattern8_1Montage = AttackPattern8_1MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackPattern9MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_AttackPattern9.AM_AttackPattern9'"));
	if (AttackPattern9MontageRef.Object)
	{
		AttackPattern9Montage = AttackPattern9MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Pattern3RotateLeft90MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_AttackPattern3_1.AM_AttackPattern3_1'"));
	if (Pattern3RotateLeft90MontageRef.Object)
	{
		Pattern3RotateLeft90Montage = Pattern3RotateLeft90MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Pattern3RotateLeft180MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_AttackPattern3_3.AM_AttackPattern3_3'"));
	if (Pattern3RotateLeft180MontageRef.Object)
	{
		Pattern3RotateLeft180Montage = Pattern3RotateLeft180MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Pattern3RotateRight90MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_AttackPattern3_2.AM_AttackPattern3_2'"));
	if (Pattern3RotateRight90MontageRef.Object)
	{
		Pattern3RotateRight90Montage = Pattern3RotateRight90MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Pattern3RotateRight180MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_AttackPattern3_4.AM_AttackPattern3_4'"));
	if (Pattern3RotateRight180MontageRef.Object)
	{
		Pattern3RotateRight180Montage = Pattern3RotateRight180MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FlyMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_Fly.AM_Fly'"));
	if (FlyMontageRef.Object)
	{
		FlyMontage = FlyMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FlyRotateLeft90MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_FlyRotateLeft90.AM_FlyRotateLeft90'"));
	if (FlyRotateLeft90MontageRef.Object)
	{
		FlyRotateLeft90Montage = FlyRotateLeft90MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FlyRotateLeft180MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_FlyRotateLeft180.AM_FlyRotateLeft180'"));
	if (FlyRotateLeft180MontageRef.Object)
	{
		FlyRotateLeft180Montage = FlyRotateLeft180MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FlyRotateRight90MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_FlyRotateRight90.AM_FlyRotateRight90'"));
	if (FlyRotateRight90MontageRef.Object)
	{
		FlyRotateRight90Montage = FlyRotateRight90MontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FlyRotateRight180MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_FlyRotateRight180.AM_FlyRotateRight180'"));
	if (FlyRotateRight180MontageRef.Object)
	{
		FlyRotateRight180Montage = FlyRotateRight180MontageRef.Object;
	}

	// Dead Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/DamagedMontages/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AirDeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/DamagedMontages/AM_AirDead.AM_AirDead'"));
	if (AirDeadMontageRef.Object)
	{
		AirDeadMontage = AirDeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> CaptureMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/DamagedMontages/AM_CaptureReaction.AM_CaptureReaction'"));
	if (CaptureMontageRef.Object)
	{
		CaptureMontage = CaptureMontageRef.Object;
	}

	// Damaged Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/DamagedMontages/AM_HitReaction.AM_HitReaction'"));
	if (HitMontageRef.Object)
	{
		HitMontage = HitMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AirHitMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/DamagedMontages/AM_AirHitReaction.AM_AirHitReaction'"));
	if (AirHitMontageRef.Object)
	{
		AirHitMontage = AirHitMontageRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> CriticalHitMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/DamagedMontages/AM_CriticalHitReaction.AM_CriticalHitReaction'"));
	if (CriticalHitMontageRef.Object)
	{
		CriticalHitMontage = CriticalHitMontageRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AirCriticalHitMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/DamagedMontages/AM_AirCriticalHitReaction.AM_AirCriticalHitReaction'"));
	if (AirCriticalHitMontageRef.Object)
	{
		AirCriticalHitMontage = AirCriticalHitMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> KnockDownMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/DamagedMontages/AM_KnockDown.AM_KnockDown'"));
	if (KnockDownMontageRef.Object)
	{
		KnockDownMontage = KnockDownMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AirKnockDownMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/DamagedMontages/AM_AirKnockDown.AM_AirKnockDown'"));
	if (AirKnockDownMontageRef.Object)
	{
		AirKnockDownMontage = AirKnockDownMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TailCutMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/DamagedMontages/AM_TailCut.AM_TailCut'"));
	if (TailCutMontageRef.Object)
	{
		TailCutMontage = TailCutMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AirTailCutMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/DamagedMontages/AM_AirTailCut.AM_AirTailCut'"));
	if (AirTailCutMontageRef.Object)
	{
		AirTailCutMontage = AirTailCutMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AirFlashHitMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/DamagedMontages/AM_AirFlashHit.AM_AirFlashHit'"));
	if (AirFlashHitMontageRef.Object)
	{
		AirFlashHitMontage = AirFlashHitMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> StunTrapMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/DamagedMontages/AM_StunTrapReaction.AM_StunTrapReaction'"));
	if (StunTrapMontageRef.Object)
	{
		StunTrapMontage = StunTrapMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AngerRoarMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_AngerRoar.AM_AngerRoar'"));
	if (AngerRoarMontageRef.Object)
	{
		AngerRoarMontage = AngerRoarMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EscapeRoarMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_EscapeRoar.AM_EscapeRoar'"));
	if (EscapeRoarMontageRef.Object)
	{
		EscapeRoarMontage = EscapeRoarMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DragonRideReadyMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_DragonRideReady.AM_DragonRideReady'"));
	if (DragonRideReadyMontageRef.Object)
	{
		DragonRideReadyMontage = DragonRideReadyMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TurfWarReadyMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animations/AM_TurfWarReady.AM_TurfWarReady'"));
	if (TurfWarReadyMontageRef.Object)
	{
		TurfWarReadyMontage = TurfWarReadyMontageRef.Object;
	}

	// AI Controller Section
	AIControllerClass = AMHAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMHCharacterRathalos::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AMHCharacterRathalos::SetDead);
	Stat->OnHeadBreak.AddUObject(this, &AMHCharacterRathalos::SetHeadBreak);
	Stat->OnLegBreak.AddUObject(this, &AMHCharacterRathalos::SetLegBreak);
	Stat->OnTailCut.AddUObject(this, &AMHCharacterRathalos::SetTailCut);
	Stat->OnWingBreak.AddUObject(this, &AMHCharacterRathalos::SetWingBreak);
	Stat->OnCapture.AddUObject(this, &AMHCharacterRathalos::SetCaptured);
	Stat->OnStun.AddUObject(this, &AMHCharacterRathalos::SetStun);
}

bool AMHCharacterRathalos::AttackHitCheck1()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackDamage = Stat->GetStat().Pattern1;
	const FVector Start = GetMesh()->GetBoneLocation(TEXT("Tail_01"));
	FVector End;
	if (!bIsTailCut)
	{
		End = GetMesh()->GetBoneLocation(TEXT("Tail_06"));
	}
	else
	{
		End = GetMesh()->GetBoneLocation(TEXT("Tail_03"));
	}

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_MHTRACECHANNEL, FCollisionShape::MakeSphere(75.0f), Params);
	
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

//#if ENABLE_DRAW_DEBUG
//
//	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
//	float CapsultHalfHeight = FVector::Distance(Start, End) * 0.5f;
//	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
//
//	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsultHalfHeight, 75.0f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 1.0f);
//
//#endif

	return HitDetected;
}

bool AMHCharacterRathalos::AttackHitCheck2()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackDamage = Stat->GetStat().Pattern2;
	const FVector Start = GetMesh()->GetBoneLocation(TEXT("Root"));
	const FVector End = GetMesh()->GetBoneLocation(TEXT("Spine_00"));

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_MHTRACECHANNEL, FCollisionShape::MakeSphere(350.0f), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}
	
//#if ENABLE_DRAW_DEBUG
//
//	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
//	float CapsultHalfHeight = FVector::Distance(Start, End) * 0.5f;
//	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
//
//	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsultHalfHeight, 350.0f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 1.0f);
//
//#endif

	return HitDetected;
}

bool AMHCharacterRathalos::AttackHitCheck3()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackDamage = Stat->GetStat().Pattern4;
	const FVector Start = GetMesh()->GetBoneLocation(TEXT("Neck_02"));
	const FVector End = GetMesh()->GetBoneLocation(TEXT("Tongue_00"));

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_MHTRACECHANNEL, FCollisionShape::MakeSphere(75.0f), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

//#if ENABLE_DRAW_DEBUG
//
//	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
//	float CapsultHalfHeight = FVector::Distance(Start, End) * 0.5f;
//	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
//
//	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsultHalfHeight, 75.0f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 1.0f);
//
//#endif

	return HitDetected;
}

bool AMHCharacterRathalos::AttackHitCheck4()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackDamage = Stat->GetStat().Pattern7;
	const FVector Start = GetMesh()->GetBoneLocation(TEXT("Bust_00"));
	const FVector End = GetMesh()->GetBoneLocation(TEXT("Tongue_00"));

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_MHTRACECHANNEL, FCollisionShape::MakeSphere(150.0f), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

//#if ENABLE_DRAW_DEBUG
//
//	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
//	float CapsultHalfHeight = FVector::Distance(Start, End) * 0.5f;
//	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
//
//	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsultHalfHeight, 150.0f, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 1.0f);
//
//#endif

	return HitDetected;
}

float AMHCharacterRathalos::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventIntigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventIntigator, DamageCauser);

	//Stat->ApplyDamage(DamageAmount);
	
	return DamageAmount;
}

void AMHCharacterRathalos::SetDead()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);

	if (GetCharacterMovement()->GetMovementName() == TEXT("Walking"))
	{
		AnimInstance->Montage_Play(DeadMontage, 1.0f);
		SetActorEnableCollision(false);
	}
	else if (GetCharacterMovement()->GetMovementName() == TEXT("Flying"))
	{
		AnimInstance->Montage_Play(AirDeadMontage, 1.0f);
	}

	AMHAIController* MHAIController = Cast<AMHAIController>(GetController());
	if (MHAIController)
	{
		MHAIController->StopAI();
	}
}

void AMHCharacterRathalos::SetCaptured()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(CaptureMontage, 1.0f);

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	SetActorEnableCollision(false);

	AMHAIController* MHAIController = Cast<AMHAIController>(GetController());
	if (MHAIController)
	{
		MHAIController->StopAI();
	}
}

void AMHCharacterRathalos::SetHeadBreak()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);

	AMHAIController* MHAIController = Cast<AMHAIController>(GetController());
	if (MHAIController)
	{
		MHAIController->StopAI();
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::RunAI);

	if (GetCharacterMovement()->GetMovementName() == TEXT("Walking"))
	{
		AnimInstance->Montage_Play(CriticalHitMontage, 1.0f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, CriticalHitMontage);
	}
	else if (GetCharacterMovement()->GetMovementName() == TEXT("Flying"))
	{
		AnimInstance->Montage_Play(AirKnockDownMontage, 1.0f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, AirKnockDownMontage);
	}
}

void AMHCharacterRathalos::SetLegBreak()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);

	AMHAIController* MHAIController = Cast<AMHAIController>(GetController());
	if (MHAIController)
	{
		MHAIController->StopAI();
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::RunAI);

	if (GetCharacterMovement()->GetMovementName() == TEXT("Walking"))
	{
		AnimInstance->Montage_Play(KnockDownMontage, 1.0f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, KnockDownMontage);
	}
	else if (GetCharacterMovement()->GetMovementName() == TEXT("Flying"))
	{
		AnimInstance->Montage_Play(AirKnockDownMontage, 1.0f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, AirKnockDownMontage);
	}
}

void AMHCharacterRathalos::SetTailCut()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);

	AMHAIController* MHAIController = Cast<AMHAIController>(GetController());
	if (MHAIController)
	{
		MHAIController->StopAI();
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::RunAI);

	if (GetCharacterMovement()->GetMovementName() == TEXT("Walking"))
	{
		AnimInstance->Montage_Play(TailCutMontage, 1.0f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, TailCutMontage);
	}
	else if (GetCharacterMovement()->GetMovementName() == TEXT("Flying"))
	{
		AnimInstance->Montage_Play(AirTailCutMontage, 1.0f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, AirTailCutMontage);
	}

	bIsTailCut = true;
}

void AMHCharacterRathalos::SetWingBreak()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);

	AMHAIController* MHAIController = Cast<AMHAIController>(GetController());
	if (MHAIController)
	{
		MHAIController->StopAI();
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::RunAI);

	if (GetCharacterMovement()->GetMovementName() == TEXT("Walking"))
	{
		AnimInstance->Montage_Play(HitMontage, 1.0f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, HitMontage);
	}
	else if (GetCharacterMovement()->GetMovementName() == TEXT("Flying"))
	{
		AnimInstance->Montage_Play(AirHitMontage, 1.0f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, AirHitMontage);
	}
}

void AMHCharacterRathalos::SetStun()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);

	AMHAIController* MHAIController = Cast<AMHAIController>(GetController());
	if (MHAIController)
	{
		MHAIController->StopAI();
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::RunAI);

	if (GetCharacterMovement()->GetMovementName() == TEXT("Walking"))
	{
		AnimInstance->Montage_Play(KnockDownMontage, 1.0f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, KnockDownMontage);
	}
	else if (GetCharacterMovement()->GetMovementName() == TEXT("Flying"))
	{
		AnimInstance->Montage_Play(AirFlashHitMontage, 1.0f);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, AirFlashHitMontage);
	}
}

void AMHCharacterRathalos::SetStunTrapEffected()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);

	AMHAIController* MHAIController = Cast<AMHAIController>(GetController());
	if (MHAIController)
	{
		MHAIController->StopAI();
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::RunAI);

	AnimInstance->Montage_Play(StunTrapMontage, 1.0f);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, StunTrapMontage);
}

void AMHCharacterRathalos::AttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	OnAttackFinished.ExecuteIfBound();
}

void AMHCharacterRathalos::RotateEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	OnRotateFinished.ExecuteIfBound();
}

void AMHCharacterRathalos::Rotate2End(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	OnRotate2Finished.ExecuteIfBound();
}

void AMHCharacterRathalos::MoveEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	OnMoveFinished.ExecuteIfBound();
}

void AMHCharacterRathalos::RoarEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	OnRoarFinished.ExecuteIfBound();
}

void AMHCharacterRathalos::Pattern3RotateEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	OnRotate2Finished.ExecuteIfBound();
}

void AMHCharacterRathalos::FlyEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	OnFlyFinished.ExecuteIfBound();
}

void AMHCharacterRathalos::FlyRotateEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	OnFlyRotateFinished.ExecuteIfBound();
}

void AMHCharacterRathalos::RunAI(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	AMHAIController* MHAIController = Cast<AMHAIController>(GetController());
	if (MHAIController)
	{
		MHAIController->RunAI();
	}
}

void AMHCharacterRathalos::EscapeRotateEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	OnEscapeRotateFinished.ExecuteIfBound();
}

void AMHCharacterRathalos::EscapeMoveEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	OnEscapeMoveFinished.ExecuteIfBound();
}

float AMHCharacterRathalos::GetAIPatrolRadius()
{
	return Stat->GetStat().PatrolRadius;
}

float AMHCharacterRathalos::GetAIDetectRange()
{
	return Stat->GetStat().DetectRange;
}

float AMHCharacterRathalos::GetAIAttackShortRange()
{
	return Stat->GetStat().AttackShortRange;
}

float AMHCharacterRathalos::GetAIAttackLongRange()
{
	return Stat->GetStat().AttackLongRange;
}

float AMHCharacterRathalos::GetAILostTargetDistance()
{
	return Stat->GetStat().LostTargetDistance;
}

void AMHCharacterRathalos::SetAIAttackDelegate(const FAIRathalosAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AMHCharacterRathalos::SetAIRotateDelegate(const FAIRathalosRotateFinished& InOnRotateFinished)
{
	OnRotateFinished = InOnRotateFinished;
}

void AMHCharacterRathalos::SetAIRotate2Delegate(const FAIRathalosRotate2Finished& InOnRotate2Finished)
{
	OnRotate2Finished = InOnRotate2Finished;
}

void AMHCharacterRathalos::SetAIMoveDelegate(const FAIRathalosMoveFinished& InOnMoveFinished)
{
	OnMoveFinished = InOnMoveFinished;
}

void AMHCharacterRathalos::SetAIRoarDelegate(const FAIRathalosRoarFinished& InOnRoarFinished)
{
	OnRoarFinished = InOnRoarFinished;
}

void AMHCharacterRathalos::SetAIFlyDelegate(const FAIRathalosFlyFinished& InOnFlyFinished)
{
	OnFlyFinished = InOnFlyFinished;
}

void AMHCharacterRathalos::SetAIFlyRotateDelegate(const FAIRathalosFlyRotateFinished& InOnFlyRotateFinished)
{
	OnFlyRotateFinished = InOnFlyRotateFinished;
}

void AMHCharacterRathalos::SetAIEscapeRotateDelegate(const FAIRathalosEscapeRotateFinished& InOnEscapeRotateFinished)
{
	OnEscapeRotateFinished = InOnEscapeRotateFinished;
}

void AMHCharacterRathalos::SetAIEscapeMoveDelegate(const FAIRathalosEscapeMoveFinished& InOnEscapeMoveFinished)
{
	OnEscapeMoveFinished = InOnEscapeMoveFinished;
}

void AMHCharacterRathalos::AttackPattern1AI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackPattern1Montage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::AttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackPattern1Montage);
}

void AMHCharacterRathalos::AttackPattern2AI()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackPattern2Montage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::AttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackPattern2Montage);
}

void AMHCharacterRathalos::AttackPattern3AI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackPattern3Montage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::AttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackPattern3Montage);
}

void AMHCharacterRathalos::AttackPattern4AI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackPattern4Montage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::AttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackPattern4Montage);
}

void AMHCharacterRathalos::AttackPattern5AI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackPattern5Montage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::AttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackPattern5Montage);
}

void AMHCharacterRathalos::AttackPattern5_1AI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackPattern5_1Montage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::AttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackPattern5_1Montage);
}

void AMHCharacterRathalos::AttackPattern5_2AI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackPattern5_2Montage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::AttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackPattern5_2Montage);
}

void AMHCharacterRathalos::AttackPattern6AI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackPattern6Montage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::AttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackPattern6Montage);
}

void AMHCharacterRathalos::AttackPattern7AI()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackPattern7Montage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::AttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackPattern7Montage);
}

void AMHCharacterRathalos::AttackPattern8AI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackPattern8Montage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::AttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackPattern8Montage);
}

void AMHCharacterRathalos::AttackPattern8_1AI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackPattern8_1Montage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::AttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackPattern8_1Montage);
}

void AMHCharacterRathalos::AttackPattern9AI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AttackPattern9Montage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::AttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AttackPattern9Montage);
}

void AMHCharacterRathalos::FlyAI()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(FlyMontage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::FlyEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, FlyMontage);
}

void AMHCharacterRathalos::FlyRotateLeft90AI(float InAngleRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(FlyRotateLeft90Montage, 1.0f, EMontagePlayReturnType::MontageLength, InAngleRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::Rotate2End);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, FlyRotateLeft90Montage);
}

void AMHCharacterRathalos::FlyRotateLeft180AI(float InAngleRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(FlyRotateLeft180Montage, 1.0f, EMontagePlayReturnType::MontageLength, InAngleRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::Rotate2End);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, FlyRotateLeft180Montage);
}

void AMHCharacterRathalos::FlyRotateRight90AI(float InAngleRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(FlyRotateRight90Montage, 1.0f, EMontagePlayReturnType::MontageLength, InAngleRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::Rotate2End);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, FlyRotateRight90Montage);
}

void AMHCharacterRathalos::FlyRotateRight180AI(float InAngleRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(FlyRotateRight180Montage, 1.0f, EMontagePlayReturnType::MontageLength, InAngleRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::Rotate2End);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, FlyRotateRight180Montage);
}

void AMHCharacterRathalos::RotateLeft90AI(float InAngleRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(RotateLeft90Montage, 1.0f, EMontagePlayReturnType::MontageLength, InAngleRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::RotateEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, RotateLeft90Montage);
}

void AMHCharacterRathalos::RotateLeft180AI(float InAngleRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(RotateLeft180Montage, 1.0f, EMontagePlayReturnType::MontageLength, InAngleRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::RotateEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, RotateLeft180Montage);
}

void AMHCharacterRathalos::RotateRight90AI(float InAngleRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(RotateRight90Montage, 1.0f, EMontagePlayReturnType::MontageLength, InAngleRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::RotateEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, RotateRight90Montage);
}

void AMHCharacterRathalos::RotateRight180AI(float InAngleRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(RotateRight180Montage, 1.0f, EMontagePlayReturnType::MontageLength, InAngleRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::RotateEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, RotateRight180Montage);
}

void AMHCharacterRathalos::Rotate2Left90AI(float InAngleRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(Rotate2Left90Montage, 1.0f, EMontagePlayReturnType::MontageLength, InAngleRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::Rotate2End);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, Rotate2Left90Montage);
}

void AMHCharacterRathalos::Rotate2Left180AI(float InAngleRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(Rotate2Left180Montage, 1.0f, EMontagePlayReturnType::MontageLength, InAngleRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::Rotate2End);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, Rotate2Left180Montage);
}

void AMHCharacterRathalos::Rotate2Right90AI(float InAngleRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(Rotate2Right90Montage, 1.0f, EMontagePlayReturnType::MontageLength, InAngleRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::Rotate2End);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, Rotate2Right90Montage);
}

void AMHCharacterRathalos::Rotate2Right180AI(float InAngleRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(Rotate2Right180Montage, 1.0f, EMontagePlayReturnType::MontageLength, InAngleRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::Rotate2End);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, Rotate2Right180Montage);
}

void AMHCharacterRathalos::EscapeRotateLeft90AI(float InAngleRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(RotateLeft90Montage, 1.0f, EMontagePlayReturnType::MontageLength, InAngleRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::EscapeRotateEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, RotateLeft90Montage);
}

void AMHCharacterRathalos::EscapeRotateLeft180AI(float InAngleRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(RotateLeft180Montage, 1.0f, EMontagePlayReturnType::MontageLength, InAngleRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::EscapeRotateEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, RotateLeft180Montage);
}

void AMHCharacterRathalos::EscapeRotateRight90AI(float InAngleRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(RotateRight90Montage, 1.0f, EMontagePlayReturnType::MontageLength, InAngleRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::EscapeRotateEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, RotateRight90Montage);
}

void AMHCharacterRathalos::EscapeRotateRight180AI(float InAngleRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(RotateRight180Montage, 1.0f, EMontagePlayReturnType::MontageLength, InAngleRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::EscapeRotateEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, RotateRight180Montage);
}

void AMHCharacterRathalos::EscapeMoveForwardAI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(DetectMoveForwardMontage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::EscapeMoveEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, DetectMoveForwardMontage);
}

void AMHCharacterRathalos::NoDetectMoveForwardAI(float InDistanceRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(NoDetectMoveForwardMontage, 1.0f, EMontagePlayReturnType::MontageLength, InDistanceRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::MoveEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, NoDetectMoveForwardMontage);
}

void AMHCharacterRathalos::DetectMoveForwardAI(float InDistanceRate)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(DetectMoveForwardMontage, 1.0f, EMontagePlayReturnType::MontageLength, InDistanceRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::MoveEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, DetectMoveForwardMontage);
}

void AMHCharacterRathalos::RoarAI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(RoarMontage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::RoarEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, RoarMontage);
}

void AMHCharacterRathalos::AngerRoarAI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(AngerRoarMontage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::RoarEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, AngerRoarMontage);
}

void AMHCharacterRathalos::EscapeRoarAI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(EscapeRoarMontage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::RoarEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, EscapeRoarMontage);
}

void AMHCharacterRathalos::DragonRideReadyAI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(DragonRideReadyMontage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::AttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, DragonRideReadyMontage);
}

void AMHCharacterRathalos::TurfWarReadyAI()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(TurfWarReadyMontage);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AMHCharacterRathalos::AttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, TurfWarReadyMontage);
}

void AMHCharacterRathalos::StopMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance->Montage_IsPlaying(NULL))
	{
		AnimInstance->StopAllMontages(0.5f);
	}
}

bool AMHCharacterRathalos::IsHp75AI()
{
	return Stat->IsHp75Percent();
}

void AMHCharacterRathalos::SetMovementModeWalking()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AMHCharacterRathalos::SetNoCollision()
{
	SetActorEnableCollision(false);
}

bool AMHCharacterRathalos::GetIsFlying()
{
	return GetCharacterMovement()->GetMovementName() == TEXT("Flying") ? true : false;
}

bool AMHCharacterRathalos::Get270secElapsedAI()
{
	return b270secElapsed;
}

bool AMHCharacterRathalos::GetIsDragonRiding()
{
	return bIsDragonRiding;
}

