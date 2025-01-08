// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/ANS_AttackCpp.h"
#include "Interface/MH_AnimNotifyInterface.h"


void UANS_AttackCpp::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp)
	{
		IMH_AnimNotifyInterface* CharacterPlayer = Cast<IMH_AnimNotifyInterface>(MeshComp->GetOwner());
		if (CharacterPlayer)
		{
			CharacterPlayer->AttackBegin();
		}
	}
}

void UANS_AttackCpp::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (MeshComp)
	{
		IMH_AnimNotifyInterface* CharacterPlayer = Cast<IMH_AnimNotifyInterface>(MeshComp->GetOwner());
		if (CharacterPlayer)
		{
			CharacterPlayer->AttackTick(AtkStartSocket, AtkEndSocket, AtkRadius);
		}
	}
}

void UANS_AttackCpp::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IMH_AnimNotifyInterface* CharacterPlayer = Cast<IMH_AnimNotifyInterface>(MeshComp->GetOwner());
		if (CharacterPlayer)
		{
			CharacterPlayer->AttackEnd();
		}
	}
}
