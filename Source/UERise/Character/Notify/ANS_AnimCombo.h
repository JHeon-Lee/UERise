// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Interface/MH_AnimNotifyInterface.h"
#include "ANS_AnimCombo.generated.h"

/**
 * 
 */
UCLASS()
class UERISE_API UANS_AnimCombo : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editable")
	TMap<EKeyInfo, TObjectPtr<class UAnimMontage>> MontageMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editable")
	bool IsChargeAtk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Editable")
	FName SectionName;	
};
