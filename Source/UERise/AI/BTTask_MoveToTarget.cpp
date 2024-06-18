// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MoveToTarget.h"
#include "MHAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"

UBTTask_MoveToTarget::UBTTask_MoveToTarget()
{
}

EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    APawn* Target;
    FVector TargetPos;
    if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET))
    {
        Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
        TargetPos = Target->GetActorLocation();
    }

    FAIRathalosMoveFinished OnMoveFinished;
    OnMoveFinished.BindLambda(
        [&]()
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    );
    AIPawn->SetAIMoveDelegate(OnMoveFinished);

    double Distance = FVector::Distance(TargetPos, Origin);

    AIPawn->DetectMoveForwardAI(1.6 - (Distance / 2900 * 1.6));
    return EBTNodeResult::InProgress;
}
