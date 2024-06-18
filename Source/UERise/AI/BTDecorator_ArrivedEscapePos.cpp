// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_ArrivedEscapePos.h"
#include "MHAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"

UBTDecorator_ArrivedEscapePos::UBTDecorator_ArrivedEscapePos()
{
}

bool UBTDecorator_ArrivedEscapePos::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	IMHRathalosAIInterface* AIPawn = Cast<IMHRathalosAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return false;
	}

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
    if (nullptr == NavSystem)
    {
        return false;
    }

    FVector Origin = ControllingPawn->GetActorLocation();
	FVector Destination = FVector(-14180, -49690, -7440);
	int32 Count = OwnerComp.GetBlackboardComponent()->GetValueAsInt(BBKEY_DESTINATIONCOUNT);
	UNavigationPath* NavPath = NavSystem->FindPathToLocationSynchronously(ControllingPawn->GetWorld(), Origin, Destination, ControllingPawn);

	float DistanceToTarget = FVector::Distance(Origin, Destination);

	if (NavPath)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_ESCAPEPOS, NavPath->PathPoints[1]);
		DrawDebugLine(ControllingPawn->GetWorld(), Origin, NavPath->PathPoints[1], FColor::Red, true, 5.0f, 0, 5.0f);
	}

	if (NavPath->PathPoints[1] == Destination)
	{
		Count++;
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(BBKEY_DESTINATIONCOUNT, Count);
	}

	if (Count >= 2 && DistanceToTarget < 4000.0f)
	{
		bResult = true;
	}
	else
	{
		bResult = false;
	}

	return bResult;
}
