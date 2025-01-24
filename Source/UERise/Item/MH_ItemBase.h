// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Components/SphereComponent.h"
#include "MH_ItemBase.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API AMH_ItemBase : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMH_ItemBase();


private:
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
							  AActor* OtherActor,
							  UPrimitiveComponent* OtherComp,
							  int32 OtherBodyIndex,
							  bool bFromSweep,
							  const FHitResult& SweepResult);
	
public:
	bool DoOnce = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UMH_ItemComponent> ItemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USphereComponent> SphereCollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TObjectPtr<class UWidgetComponent> ItemWidgetComponent;

};
