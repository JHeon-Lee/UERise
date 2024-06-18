// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RotateToPos.h"
#include "MHAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"

UBTTask_RotateToPos::UBTTask_RotateToPos()
{
}

EBTNodeResult::Type UBTTask_RotateToPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
    FVector ForwardVec = ControllingPawn->GetActorForwardVector();
    FVector PatrolPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_PATROLPOS);

    FAIRathalosRotate2Finished OnRotateFinished;
    OnRotateFinished.BindLambda(
        [&]()
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    );
    AIPawn->SetAIRotate2Delegate(OnRotateFinished);

    FVector VectorToTarget = PatrolPos - Origin;
    VectorToTarget.Normalize(1.0f);
    float AngleBetweenTarget = acosf(FVector::DotProduct(VectorToTarget, ForwardVec));
    float CrossZ = FVector::CrossProduct(ForwardVec, VectorToTarget).Z;
    
    if (CrossZ < 0)
    {
        if (AngleBetweenTarget > 0 && AngleBetweenTarget < (PI / 2))
        {
            AIPawn->Rotate2Left90AI(0.8 - (AngleBetweenTarget / (PI / 2) * 0.8));
            return EBTNodeResult::InProgress;
        }
        else if (AngleBetweenTarget > (PI / 2) && AngleBetweenTarget < PI)
        {
            AIPawn->Rotate2Left180AI(1.1 - (AngleBetweenTarget / PI * 1.1));
            return EBTNodeResult::InProgress;
        }
        else 
            return EBTNodeResult::Succeeded;
    }
    else
    {
        if (AngleBetweenTarget > 0 && AngleBetweenTarget < (PI / 2))
        {
            AIPawn->Rotate2Right90AI(0.8 - (AngleBetweenTarget / (PI / 2) * 0.8));
            return EBTNodeResult::InProgress;
        }
        else if (AngleBetweenTarget > (PI / 2) && AngleBetweenTarget < PI)
        {
            AIPawn->Rotate2Right180AI(1.1 - (AngleBetweenTarget / PI * 1.1));
            return EBTNodeResult::InProgress;
        }
        else
            return EBTNodeResult::Succeeded;
    }
}
