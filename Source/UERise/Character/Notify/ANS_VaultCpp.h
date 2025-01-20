// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_VaultCpp.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UANS_VaultCpp : public UAnimNotifyState
{
	GENERATED_BODY()
	UANS_VaultCpp():ValutOffset(0.0f) {}

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ValutOffset;

};
