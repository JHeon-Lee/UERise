// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MoveToEscapePos.h"
#include "MHAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"

UBTTask_MoveToEscapePos::UBTTask_MoveToEscapePos()
{
}

EBTNodeResult::Type UBTTask_MoveToEscapePos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
    FVector EscapePos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ESCAPEPOS);

    FAIRathalosEscapeMoveFinished OnEscapeMoveFinished;
    OnEscapeMoveFinished.BindLambda(
        [&]()
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    );
    AIPawn->SetAIEscapeMoveDelegate(OnEscapeMoveFinished);

    AIPawn->EscapeMoveForwardAI();
    return EBTNodeResult::InProgress;
}
