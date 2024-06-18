// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MHAnimInstancePlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/MHCharacterGetterInterface.h"

UMHAnimInstancePlayer::UMHAnimInstancePlayer()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
}

void UMHAnimInstancePlayer::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UMHAnimInstancePlayer::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshold;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
	}

	IMHCharacterGetterInterface* CharacterPlayer = Cast<IMHCharacterGetterInterface>(Owner);
	if (CharacterPlayer)
	{
		bIsDrawing = CharacterPlayer->GetIsDrawing();
	}
}
