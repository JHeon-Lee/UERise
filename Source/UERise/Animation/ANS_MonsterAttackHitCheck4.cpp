// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ANS_MonsterAttackHitCheck4.h"
#include "Interface/MHMonsterAttackInterface.h"

UANS_MonsterAttackHitCheck4::UANS_MonsterAttackHitCheck4()
{
}

void UANS_MonsterAttackHitCheck4::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	bIsHitDetected = false;
}

void UANS_MonsterAttackHitCheck4::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (MeshComp)
	{
		IMHMonsterAttackInterface* AttackPawn = Cast<IMHMonsterAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			if (!bIsHitDetected)
			{
				bIsHitDetected = AttackPawn->AttackHitCheck4();
			}
		}
	}
}
