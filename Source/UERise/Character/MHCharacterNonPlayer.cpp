// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MHCharacterNonPlayer.h"

// Sets default values
AMHCharacterNonPlayer::AMHCharacterNonPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMHCharacterNonPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMHCharacterNonPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMHCharacterNonPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

