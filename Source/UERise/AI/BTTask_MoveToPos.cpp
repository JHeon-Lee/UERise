// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MoveToPos.h"
#include "MHAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"

UBTTask_MoveToPos::UBTTask_MoveToPos()
{
}

EBTNodeResult::Type UBTTask_MoveToPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (nullptr == ControllingPawn)
    {
        return EBTNodeResult::Failed;
    }

    IMHRathalosAIInterface* AIPawn = Cast<IMHRathalosAIInterface>(ControllingPawn);
    if (nullptr == AIPawn)
    {
        return EBTNodeResult::Failed;
    }

    FVector Origin = ControllingPawn->GetActorLocation();
    FVector PatrolPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_PATROLPOS);
    
    FAIRathalosMoveFinished OnMoveFinished;
    OnMoveFinished.BindLambda(
        [&]()
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    );
    AIPawn->SetAIMoveDelegate(OnMoveFinished);
    
    double Distance = FVector::Distance(PatrolPos, Origin);

    AIPawn->NoDetectMoveForwardAI(4.4 - (Distance / 1100 * 4.4));
    return EBTNodeResult::InProgress;
}
