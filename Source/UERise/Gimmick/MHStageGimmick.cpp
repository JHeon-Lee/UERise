// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/MHStageGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/MHCollision.h"
#include "Character/MHCharacterRathalos.h"


// Sets default values
AMHStageGimmick::AMHStageGimmick()
{
	// Stage Section
	Stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	RootComponent = Stage;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ImportSource/Terrain/SM_Ice_Fort_Floor_1.SM_Ice_Fort_Floor_1'"));
	if (StageMeshRef.Object)
	{
		Stage->SetStaticMesh(StageMeshRef.Object);
	}

	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	StageTrigger->SetBoxExtent(FVector(200.0f, 200.0f, 300.0f));
	StageTrigger->SetupAttachment(Stage);
	StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	StageTrigger->SetCollisionProfileName(CPROFILE_MHTRIGGER);
	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &AMHStageGimmick::OnStageTriggerBeginOverlap);

	// Monster Section
	OpponentSpawnTime = 1.0f;
	MonsterClass = AMHCharacterRathalos::StaticClass();
}

void AMHStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
}

