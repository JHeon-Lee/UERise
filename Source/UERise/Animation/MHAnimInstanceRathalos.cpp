// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MHAnimInstanceRathalos.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/MHAIController.h"

UMHAnimInstanceRathalos::UMHAnimInstanceRathalos()
{
	MovingThreshold = 1.0f;
}

void UMHAnimInstanceRathalos::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UMHAnimInstanceRathalos::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		Speed = Velocity.Size2D();
		bIsIdle = Speed < MovingThreshold;
		bIsFlying = Movement->IsFlying();
	}
}
