// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RotateToEscapePos.h"
#include "MHAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"
#include "Kismet/KismetSystemLibrary.h"

UBTTask_RotateToEscapePos::UBTTask_RotateToEscapePos()
{
}

EBTNodeResult::Type UBTTask_RotateToEscapePos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
    FVector EscapePos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ESCAPEPOS);

    FAIRathalosEscapeRotateFinished OnEscapeRotateFinished;
    OnEscapeRotateFinished.BindLambda(
        [&]()
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    );
    AIPawn->SetAIEscapeRotateDelegate(OnEscapeRotateFinished);

    FVector VectorToTarget = EscapePos - Origin;
    VectorToTarget.Normalize(1.0f);
    float AngleBetweenTarget = acosf(FVector::DotProduct(VectorToTarget, ForwardVec));
    float CrossZ = FVector::CrossProduct(ForwardVec, VectorToTarget).Z;

    FString FloatString = FString::SanitizeFloat(AngleBetweenTarget);
    UKismetSystemLibrary::PrintString(ControllingPawn->GetWorld(), *FloatString, true, true, FLinearColor::Green, 2.0f);

    if (CrossZ < 0)
    {
        if (AngleBetweenTarget > 0 && AngleBetweenTarget < (PI / 2))
        {
            AIPawn->EscapeRotateLeft90AI(1.5 - (AngleBetweenTarget / (PI / 2) * 1.5));
            return EBTNodeResult::InProgress;
        }
        else if (AngleBetweenTarget > (PI / 2) && AngleBetweenTarget < PI)
        {
            AIPawn->EscapeRotateLeft180AI(2.3 - (AngleBetweenTarget / PI * 2.1));
            return EBTNodeResult::InProgress;
        }
        else
            return EBTNodeResult::Succeeded;
    }
    else
    {
        if (AngleBetweenTarget > 0 && AngleBetweenTarget < (PI / 2))
        {
            AIPawn->EscapeRotateRight90AI(1.5 - (AngleBetweenTarget / (PI / 2) * 1.5));
            return EBTNodeResult::InProgress;
        }
        else if (AngleBetweenTarget > (PI / 2) && AngleBetweenTarget < PI)
        {
            AIPawn->EscapeRotateRight180AI(2.3 - (AngleBetweenTarget / PI * 2.1));
            return EBTNodeResult::InProgress;
        }
        else
            return EBTNodeResult::Succeeded;
    }
}
