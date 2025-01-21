// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/MHEquipmentComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UMHEquipmentComponent::UMHEquipmentComponent()
{
	static ConstructorHelpers::FObjectFinder<UMHEquipmentAsset> DataContainer(TEXT("/Script/UERise.MHEquipmentAsset'/Game/Characters/Utusi/Data/MHEquipment.MHEquipment'"));
	if (DataContainer.Object)
	{
		EquipAsset = DataContainer.Object;
	}

	Part_Helm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Helm"));
	Part_Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Part_Leg = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Leg"));
	Part_Arm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm"));
	Part_Wst = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Wst"));
}


// Called when the game starts
void UMHEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	if (EquipAsset)
	{
		Part_Helm->SetSkeletalMesh(EquipAsset->GetHelmMesh());
		Part_Body->SetSkeletalMesh(EquipAsset->GetBodyMesh());
		Part_Leg->SetSkeletalMesh(EquipAsset->GetLegMesh());
		Part_Arm->SetSkeletalMesh(EquipAsset->GetArmMesh());
		Part_Wst->SetSkeletalMesh(EquipAsset->GetWstMesh());
	}

	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		check(OwnerCharacter->GetMesh());
		OwnerMesh = OwnerCharacter->GetMesh();
	}

	ensure(OwnerMesh);
	Part_Helm->AttachToComponent(OwnerMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Part_Body->AttachToComponent(OwnerMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Part_Leg->AttachToComponent(OwnerMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Part_Arm->AttachToComponent(OwnerMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Part_Wst->AttachToComponent(OwnerMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));


	Part_Helm->SetLeaderPoseComponent(OwnerMesh);
	Part_Body->SetLeaderPoseComponent(OwnerMesh);
	Part_Leg->SetLeaderPoseComponent(OwnerMesh);
	Part_Arm->SetLeaderPoseComponent(OwnerMesh);
	Part_Wst->SetLeaderPoseComponent(OwnerMesh);
}