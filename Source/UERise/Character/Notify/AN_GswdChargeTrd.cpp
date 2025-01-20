// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/AN_GswdChargeTrd.h"
#include "Interface/MH_GswdNotifyInterface.h"

void UAN_GswdChargeTrd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	check(MeshComp);

	TArray<UActorComponent*> Components;
	MeshComp->GetOwner()->GetComponents(Components);

	for (UActorComponent* Component : Components)
	{
		if (IMH_GswdNotifyInterface* Interface = Cast<IMH_GswdNotifyInterface>(Component))
		{
			Interface->GSwdTrdCharge();
		}
	}
}
