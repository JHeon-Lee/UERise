// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FlyRotate.h"
#include "MHAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"

UBTTask_FlyRotate::UBTTask_FlyRotate()
{
}

EBTNodeResult::Type UBTTask_FlyRotate::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
    APawn* Target;
    FVector TargetPos;
    if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET))
    {
        Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
        TargetPos = Target->GetActorLocation();
    }

    FAIRathalosRotate2Finished OnRotateFinished;
    OnRotateFinished.BindLambda(
        [&]()
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    );
    AIPawn->SetAIRotate2Delegate(OnRotateFinished);

    FVector VectorToTarget = TargetPos - Origin;
    VectorToTarget.Normalize(1.0f);
    float AngleBetweenTarget = acosf(FVector::DotProduct(VectorToTarget, ForwardVec));
    float CrossZ = FVector::CrossProduct(ForwardVec, VectorToTarget).Z;

    if (CrossZ < 0)
    {
        if (AngleBetweenTarget > 0 && AngleBetweenTarget < (PI / 2))
        {
            AIPawn->FlyRotateLeft90AI(1.4 - (AngleBetweenTarget / (PI / 2) * 1.4));
            return EBTNodeResult::InProgress;
        }
        else if (AngleBetweenTarget > (PI / 2) && AngleBetweenTarget < PI)
        {
            AIPawn->FlyRotateLeft180AI(1.8 - (AngleBetweenTarget / PI * 1.8));
            return EBTNodeResult::InProgress;
        }
        else
            return EBTNodeResult::Succeeded;
    }
    else
    {
        if (AngleBetweenTarget > 0 && AngleBetweenTarget < (PI / 2))
        {
            AIPawn->FlyRotateRight90AI(1.2 - (AngleBetweenTarget / (PI / 2) * 1.2));
            return EBTNodeResult::InProgress;
        }
        else if (AngleBetweenTarget > (PI / 2) && AngleBetweenTarget < PI)
        {
            AIPawn->FlyRotateRight180AI(1.6 - (AngleBetweenTarget / PI * 1.6));
            return EBTNodeResult::InProgress;
        }
        else
            return EBTNodeResult::Succeeded;
    }
}
