// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MHAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MHAI.h"
#include "Interface/MHRathalosGetterInterface.h"

AMHAIController::AMHAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/AI/BB_MHCharacterRathalos.BB_MHCharacterRathalos'"));
	if (nullptr != BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/BT_MHCharacterRathalos.BT_MHCharacterRathalos'"));
	if (nullptr != BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AMHAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsVector(BBKEY_POS, FVector(-14050, -49480, -7540));
		// For Short Range Pattern
		Blackboard->SetValueAsVector(BBKEY_ATTACKPATTERN1, FVector(0, false, false));
		Blackboard->SetValueAsVector(BBKEY_ATTACKPATTERN2, FVector(1, false, false));
		Blackboard->SetValueAsVector(BBKEY_ATTACKPATTERN4, FVector(2, false, false));
		Blackboard->SetValueAsVector(BBKEY_ATTACKPATTERN7, FVector(3, false, false));

		// For long Range Pattern
		Blackboard->SetValueAsVector(BBKEY_ATTACKPATTERN3, FVector(0, false, true));
		Blackboard->SetValueAsVector(BBKEY_ATTACKPATTERN5_1, FVector(1, false, true));
		Blackboard->SetValueAsVector(BBKEY_ATTACKPATTERN5_2, FVector(2, false, true));
		Blackboard->SetValueAsVector(BBKEY_ATTACKPATTERN5_3, FVector(3, false, true));
		Blackboard->SetValueAsVector(BBKEY_ATTACKPATTERN6, FVector(4, false, true));
		Blackboard->SetValueAsVector(BBKEY_ATTACKPATTERN8, FVector(5, false, true));
		Blackboard->SetValueAsVector(BBKEY_ATTACKPATTERN9, FVector(6, false, true));

		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AMHAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void AMHAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	RunAI();
}
