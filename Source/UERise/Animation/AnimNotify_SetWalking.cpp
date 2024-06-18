// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_SetWalking.h"
#include "Interface/MHRathalosSetterInterface.h"

void UAnimNotify_SetWalking::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IMHRathalosSetterInterface* CharacterRathalos = Cast<IMHRathalosSetterInterface>(MeshComp->GetOwner());
		if (CharacterRathalos)
		{
			CharacterRathalos->SetMovementModeWalking();
		}
	}
}
