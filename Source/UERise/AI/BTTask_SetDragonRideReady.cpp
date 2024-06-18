// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_SetDragonRideReady.h"
#include "MHAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"

UBTTask_SetDragonRideReady::UBTTask_SetDragonRideReady()
{
}

EBTNodeResult::Type UBTTask_SetDragonRideReady::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    //ControllingPawn->SetActorLocation(); If defined the Destination Vector
    OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_HASESCAPED, true);

    FAIRathalosAttackFinished OnAttackFinished;
    OnAttackFinished.BindLambda(
        [&]()
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    );
    AIPawn->SetAIAttackDelegate(OnAttackFinished);

    AIPawn->TurfWarReadyAI(); // Don't Change
    return EBTNodeResult::InProgress;
}
