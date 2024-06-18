// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MHBaseAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UMHBaseAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	class AMH_PlayerCharacter* Owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	class UAnimInstance* OwnerAnim;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float InitialRotZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	FVector InitalPos;


};