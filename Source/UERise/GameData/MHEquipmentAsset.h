// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MHEquipmentAsset.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UMHEquipmentAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	USkeletalMesh* GetHelmMesh() {return Helm;}
	USkeletalMesh* GetBodyMesh() { return Body; }
	USkeletalMesh* GetLegMesh() { return Leg; }
	USkeletalMesh* GetArmMesh() { return Arm; }
	USkeletalMesh* GetWstMesh() { return Wst; }

	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMesh> Helm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMesh> Body;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMesh> Leg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMesh> Arm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMesh> Wst;
	
};
