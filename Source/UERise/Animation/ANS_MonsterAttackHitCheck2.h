// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_MonsterAttackHitCheck2.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UANS_MonsterAttackHitCheck2 : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UANS_MonsterAttackHitCheck2();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	bool bIsHitDetected;
};
