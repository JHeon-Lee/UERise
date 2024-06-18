// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_ChangeSocket.h"
#include "Interface/MHAnimationSocketInterface.h"

void UAnimNotify_ChangeSocket::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IMHAnimationSocketInterface* CharacterPlayer = Cast<IMHAnimationSocketInterface>(MeshComp->GetOwner());
		if (CharacterPlayer)
		{
			CharacterPlayer->ChangeWeaponLocation();
		}
	}
}
