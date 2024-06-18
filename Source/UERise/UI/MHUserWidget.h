// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MHUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UMHUserWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	FORCEINLINE void SetOwningActor(AActor* NewOwner) { OwingActor = NewOwner; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	TObjectPtr<AActor> OwingActor;


};
