// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/AN_GSwdEftOnOff.h"
#include "Interface/MH_GswdNotifyInterface.h"

void UAN_GSwdEftOnOff::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	check(MeshComp);

	TArray<UActorComponent*> Components;
	MeshComp->GetOwner()->GetComponents(Components);

	for (UActorComponent* Component : Components)
	{
		if (IMH_GswdNotifyInterface* Interface = Cast<IMH_GswdNotifyInterface>(Component))
		{
			if (IsTurnOn)
			{
				Interface->PostProcessEffectOn();
			}
			else
			{
				Interface->PostProcessEffectOff();
			}
		}
	}
}
