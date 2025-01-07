// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/ANS_AnimCombo.h"
#include "Interface/MH_AnimNotifyInterface.h"

void UANS_AnimCombo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (MeshComp)
	{
		IMH_AnimNotifyInterface* CharacterPlayer = Cast<IMH_AnimNotifyInterface>(MeshComp->GetOwner());
		if (CharacterPlayer)
		{
			CharacterPlayer->ComboTick(MontageMap, IsChargeAtk, SectionName);
		}
	}
}
