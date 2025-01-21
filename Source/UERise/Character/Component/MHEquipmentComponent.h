// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/MHEquipmentAsset.h"
#include "MHEquipmentComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UERISE_API UMHEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMHEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	TObjectPtr<UMHEquipmentAsset> EquipAsset;

	UPROPERTY()
	TObjectPtr<class USkeletalMeshComponent> OwnerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> Part_Helm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> Part_Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> Part_Leg;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> Part_Arm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> Part_Wst;
};
