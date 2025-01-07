// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_ShootWireBugCpp.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UANS_ShootWireBugCpp : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editable")
	float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editable")
	float ZDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editable")
	float MoveTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editable")
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editable")
	float FDistance;
	
};
