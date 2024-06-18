// Fill out your copyright notice in the Description page of Project Settings.


#include "MHCharacterMonster.h"

// Sets default values
AMHCharacterMonster::AMHCharacterMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMHCharacterMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMHCharacterMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMHCharacterMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

