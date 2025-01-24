// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Notify/ANS_Rotate.h"
#include "Interface/MH_AnimNotifyInterface.h"

void UANS_Rotate::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* Actor = MeshComp->GetOwner();

	if (ensure(IsValid(Actor)))
	{
		InitalYaw = Actor->GetActorRotation().Yaw;
	}
}

void UANS_Rotate::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (MeshComp)
	{
		IMH_AnimNotifyInterface* CharacterPlayer = Cast<IMH_AnimNotifyInterface>(MeshComp->GetOwner());
		if (CharacterPlayer)
		{
			CharacterPlayer->RotateTick(InitalYaw, MaxRotateDegree, RotateSpeed);
		}
	}

}
