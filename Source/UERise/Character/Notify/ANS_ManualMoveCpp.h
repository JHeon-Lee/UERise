// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_ManualMoveCpp.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UANS_ManualMoveCpp : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editable")
	float ManualMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editable")
	float ManualMoveZSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editable")
	float FlymodeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editable")
	bool WallRunning;
};
