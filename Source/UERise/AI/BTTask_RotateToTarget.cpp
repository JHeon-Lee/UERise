// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RotateToTarget.h"
#include "MHAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"

UBTTask_RotateToTarget::UBTTask_RotateToTarget()
{
}

EBTNodeResult::Type UBTTask_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    AIPawn->StopMontage();

    FAIRathalosRotateFinished OnRotateFinished;
    OnRotateFinished.BindLambda(
        [&]()
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    );
    AIPawn->SetAIRotateDelegate(OnRotateFinished);

    FVector VectorToTarget = TargetPos - Origin;
    VectorToTarget.Normalize(1.0f);
    float AngleBetweenTarget = acosf(FVector::DotProduct(VectorToTarget, ForwardVec));
    float CrossZ = FVector::CrossProduct(ForwardVec, VectorToTarget).Z;
    
    //DrawDebugLine(ControllingPawn->GetWorld(), Origin, TargetPos, FColor::Red, true, 5.0f, 0, 5.0f);

    if (CrossZ < 0)
    {
        if (AngleBetweenTarget > 0 && AngleBetweenTarget < (PI / 2))
        {
            AIPawn->RotateLeft90AI(1.5 - (AngleBetweenTarget / (PI / 2) * 1.5));
            return EBTNodeResult::InProgress;
        }
        else if (AngleBetweenTarget > (PI / 2) && AngleBetweenTarget < PI)
        {
            AIPawn->RotateLeft180AI(2.3 - (AngleBetweenTarget / PI * 2.1));
            return EBTNodeResult::InProgress;
        }
        else
            return EBTNodeResult::Succeeded;
    }
    else
    {
        if (AngleBetweenTarget > 0 && AngleBetweenTarget < (PI / 2))
        {
            AIPawn->RotateRight90AI(1.5 - (AngleBetweenTarget / (PI / 2) * 1.5));
            return EBTNodeResult::InProgress;
        }
        else if (AngleBetweenTarget > (PI / 2) && AngleBetweenTarget < PI)
        {
            AIPawn->RotateRight180AI(2.3 - (AngleBetweenTarget / PI * 2.1));
            return EBTNodeResult::InProgress;
        }
        else
            return EBTNodeResult::Succeeded;
    }
}
