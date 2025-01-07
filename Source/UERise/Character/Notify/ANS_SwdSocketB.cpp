// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/ANS_SwdSocketB.h"
#include "Interface/MH_AnimNotifyInterface.h"

void UANS_SwdSocketB::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IMH_AnimNotifyInterface* CharacterPlayer = Cast<IMH_AnimNotifyInterface>(MeshComp->GetOwner());
		if (CharacterPlayer)
		{
			CharacterPlayer->SwdAttachToSocket(SocketName);
		}
	}
}