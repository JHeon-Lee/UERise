// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckDistance.h"
#include "MHAI.h"
#include "AIController.h"
#include "Interface/MHRathalosAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_CheckDistance::UBTService_CheckDistance()
{
	NodeName = TEXT("CheckLostTargetDistance");
	Interval = 1.0f;
}

void UBTService_CheckDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	IMHRathalosAIInterface* AIPawn = Cast<IMHRathalosAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}

	float LostTargetDistance = AIPawn->GetAILostTargetDistance();

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
	{
		return;
	}

	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);

//	DrawDebugSphere(World, Center, LostTargetDistance, 16, FColor::Green, false, 0.2f);

	if (DistanceToTarget > LostTargetDistance)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_ISDETECTING, false);
	}
}
