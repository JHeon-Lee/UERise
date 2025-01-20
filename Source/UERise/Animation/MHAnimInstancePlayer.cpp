// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MHAnimInstancePlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/MHCharacterGetterInterface.h"

UMHAnimInstancePlayer::UMHAnimInstancePlayer()
{
}

void UMHAnimInstancePlayer::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	A_Utusi = Cast<AMH_PlayerCharacter>(GetOwningActor());

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
		bFalling = Movement->IsFalling();
		bOnGround = Movement->IsMovingOnGround();		
	}

//	IMHCharacterGetterInterface* CharacterPlayer = Cast<IMHCharacterGetterInterface>(Owner);
	if (A_Utusi)
	{		
		bPressWASD = A_Utusi->PressWASD;
		bPressRB = A_Utusi->PressRB;
		bPressA = A_Utusi->PressA;
		bPressY = A_Utusi->PressY;
		bPressX = A_Utusi->PressX;
		weaponType = A_Utusi->WeaponType;
	}
}
