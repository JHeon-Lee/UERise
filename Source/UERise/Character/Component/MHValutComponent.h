// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/MHGlobalEnum.h"
#include "MHValutComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCanPlayMonate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UERISE_API UMHValutComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMHValutComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsNearWall() { return  !InitialImpactPoint.IsZero(); }
	float GetWallDimensionZ() { return ImpactPoint.Z; }
	void MakePlayerStickToWall();

	void SetCanPlayValutMontage(bool Can) {CanPlayValutMontage = Can;}

public:
	FOnCanPlayMonate CanPlayMontageDelegate;


protected:	
	// Valut
	void ValutCheck();
	void GetFrontObjectLocation();
	void GetObjectDimension();
	void PlayValutMontatge();




protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	float TargetRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	float DistanceToWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	float VaultOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	float WallSlope;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	float WallRotZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	FVector ImpactPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	FVector InitialImpactPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	FVector WarpPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	FVector ImpactNormal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool CanWallRun;

	UPROPERTY()
	bool CanPlayValutMontage;

};
