// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/AN_GwdChargeFst.h"
#include "Interface/MH_AnimNotifyInterface.h"

void UAN_GwdChargeFst::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IMH_AnimNotifyInterface* CharacterPlayer = Cast<IMH_AnimNotifyInterface>(MeshComp->GetOwner());
		if (CharacterPlayer)
		{
			CharacterPlayer->GSwdFstCharge();
		}
	}
}
