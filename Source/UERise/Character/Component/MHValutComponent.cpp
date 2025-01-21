// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/MHValutComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MHGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UMHValutComponent::UMHValutComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1;
	// ...
}


// Called when the game starts
void UMHValutComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMHValutComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ValutCheck();
	// ...
}

void UMHValutComponent::ValutCheck()
{
	GetFrontObjectLocation();	

	if (DistanceToWall > 0.0f && DistanceToWall <= 100.0f)
	{

		if (CanPlayValutMontage)
		{
			PlayValutMontatge();
		}
	}
}

void UMHValutComponent::GetFrontObjectLocation()
{
	FVector TraceStart = GetOwner()->GetActorLocation() - FVector(0, 0, 30);
	FVector TraceEnd = TraceStart + GetOwner()->GetActorRightVector() * 100;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeArray;
	ObjectTypeArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TArray<TObjectPtr<AActor>> IgnoreActors;
	FHitResult TraceResult;

	UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		TraceStart,
		TraceEnd,
		10.0f,
		ObjectTypeArray,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		TraceResult,
		true
	);

	if (TraceResult.bBlockingHit)
	{
		ImpactNormal = TraceResult.ImpactNormal;
		WallRotZ = TraceResult.ImpactNormal.ToOrientationRotator().Yaw + 90.0f;
		WallSlope = UKismetMathLibrary::DegAcos(FVector::DotProduct(TraceResult.ImpactNormal, GetOwner()->GetActorUpVector()));
		InitialImpactPoint = TraceResult.ImpactPoint;
		DistanceToWall = FVector::Distance(GetOwner()->GetActorLocation(), InitialImpactPoint);
		GetObjectDimension();
	}
	else
	{
		InitialImpactPoint = FVector(0, 0, 0);
		DistanceToWall = 0.0;
		CanWallRun = false;
	}

}

void UMHValutComponent::GetObjectDimension()
{
	const uint8 LastIndex = 30;
	uint8 CurIndex = 0;

	for (; CurIndex <= LastIndex; CurIndex++)
	{
		FVector TraceStart = InitialImpactPoint;
		TraceStart.Z += CurIndex * 15;

		FVector TraceEnd = TraceStart + GetOwner()->GetActorRightVector() * 150;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeArray;
		ObjectTypeArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		TArray<TObjectPtr<AActor>> IgnoreActors;
		FHitResult TraceResult;

		UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			TraceStart,
			TraceEnd,
			ObjectTypeArray,
			false,
			IgnoreActors,
			EDrawDebugTrace::None,
			TraceResult,
			true
		);

		if (TraceResult.bBlockingHit)
		{
			ImpactPoint = TraceResult.ImpactPoint;
		}
		else
		{
			break;
		}
	}
	
}

void UMHValutComponent::PlayValutMontatge()
{
	float DistanceToVaultPoint = ImpactPoint.Z - GetOwner()->GetActorLocation().Z;

//	if (DistanceToVaultPoint > 150.0f)
//	{
//		ValutMontageDelegate.Broadcast(EValutMontage::WallRun);		
//	}
//	else if (UKismetMathLibrary::InRange_FloatFloat(DistanceToVaultPoint, 40.0f, 150.0f, false))
//	{
//	//	VaultOffset = 20.0f;
//		ValutMontageDelegate.Broadcast(EValutMontage::JumpOver);
//	}
//	else if (UKismetMathLibrary::InRange_FloatFloat(DistanceToVaultPoint, -40.0f, 40.0f))
//	{
//	//	VaultOffset = -45.0f;
//		ValutMontageDelegate.Broadcast(EValutMontage::Vault);
//	}

}

void UMHValutComponent::MakePlayerStickToWall()
{
	if (!InitialImpactPoint.IsZero())
	{
		FVector HitPoint = (GetOwner()->GetActorRightVector() * -50) + InitialImpactPoint;
		HitPoint.Z = GetOwner()->GetActorLocation().Z;
		GetOwner()->SetActorLocation(HitPoint);

		FVector RotVector = FVector(ImpactNormal.X, ImpactNormal.Y, 0.0);
		FRotator Rot = RotVector.ToOrientationRotator();
		Rot.Yaw += 90.0f;
		GetOwner()->SetActorRotation(Rot);
	}
}

