// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_AngerRoar.h"
#include "MHAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"

UBTTask_AngerRoar::UBTTask_AngerRoar()
{
}

EBTNodeResult::Type UBTTask_AngerRoar::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    FAIRathalosRoarFinished OnRoarFinished;
    OnRoarFinished.BindLambda(
        [&]()
        {
            OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_HASANGERED, true);
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    );
    AIPawn->SetAIRoarDelegate(OnRoarFinished);

    AIPawn->AngerRoarAI();
    return EBTNodeResult::InProgress;
}
