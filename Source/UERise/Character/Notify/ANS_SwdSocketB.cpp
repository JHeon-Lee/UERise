// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/ANS_SwdSocketB.h"
#include "Interface/MH_GswdNotifyInterface.h"

void UANS_SwdSocketB::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	check(MeshComp);

	TArray<UActorComponent*> Components; 
	MeshComp->GetOwner()->GetComponents(Components);

	for (UActorComponent* Component : Components)
	{
		if (IMH_GswdNotifyInterface* Interface = Cast<IMH_GswdNotifyInterface>(Component))
		{
			Interface->SwdAttachToSocket(SocketName);
		}
	}
}