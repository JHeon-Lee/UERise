// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_AttackCpp.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UANS_AttackCpp : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editable")
	FName AtkStartSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editable")
	FName AtkEndSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editable")
	float AtkRadius;
};
