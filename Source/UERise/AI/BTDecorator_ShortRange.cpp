// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_ShortRange.h"
#include "MHAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/MHRathalosAIInterface.h"

UBTDecorator_ShortRange::UBTDecorator_ShortRange()
{
	NodeName = TEXT("ShortRange");
}

bool UBTDecorator_ShortRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	IMHRathalosAIInterface* AIPawn = Cast<IMHRathalosAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
	{
		return false;
	}

	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	float AttackShortRange = AIPawn->GetAIAttackShortRange();
	float AttackLongRange = AIPawn->GetAIAttackLongRange();
	bool bInShortRange = (DistanceToTarget <= AttackShortRange);
	bool bInLongRange = (DistanceToTarget <= AttackLongRange) && (DistanceToTarget > AttackShortRange);

	if (bInShortRange)
	{
		TArray<FVector> ShortRangePatterns;

		int32 Count = 0;
		FVector Vec1 = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN1);
		FVector Vec2 = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN2);
		FVector Vec3 = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN4);
		FVector Vec4 = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN7);
		
		if (Vec1.Y == 0)
		{
			ShortRangePatterns.Emplace(Vec1);
			Count++;
		}
		if (Vec2.Y == 0)
		{
			ShortRangePatterns.Emplace(Vec2);
			Count++;
		}
		if (Vec3.Y == 0)
		{
			ShortRangePatterns.Emplace(Vec3);
			Count++;
		}
		if (Vec4.Y == 0)
		{
			ShortRangePatterns.Emplace(Vec4);
			Count++;
		}

		if (Count == 0)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_ATTACKPATTERN1, FVector(0, false, false));
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_ATTACKPATTERN2, FVector(1, false, false));
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_ATTACKPATTERN4, FVector(2, false, false));
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_ATTACKPATTERN7, FVector(3, false, false));
			
			ShortRangePatterns.Emplace(OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN1));
			ShortRangePatterns.Emplace(OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN2));
			ShortRangePatterns.Emplace(OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN4));
			ShortRangePatterns.Emplace(OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN7));

			int32 Index = FMath::RandRange(0, 3);
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(BBKEY_ATTACKPATTERNSELECTED, ShortRangePatterns[Index].X);
		}
		else
		{
			int32 Index = FMath::RandRange(0, Count - 1);
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(BBKEY_ATTACKPATTERNSELECTED, ShortRangePatterns[Index].X);
		}
	}
	else if (bInLongRange)
	{
		TArray<FVector> LongRangePatterns;

		int32 Count = 0;
		FVector Vec1 = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN3);
		FVector Vec2 = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN5_1);
		FVector Vec3 = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN5_2);
		FVector Vec4 = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN5_3);
		FVector Vec5 = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN6);
		FVector Vec6 = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN8);
		FVector Vec7 = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN9);
		
		if (Vec1.Y == 0)
		{
			LongRangePatterns.Emplace(Vec1);
			Count++;
		}
		if (Vec2.Y == 0)
		{
			LongRangePatterns.Emplace(Vec2);
			Count++;
		}
		if (Vec3.Y == 0)
		{
			LongRangePatterns.Emplace(Vec3);
			Count++;
		}
		if (Vec4.Y == 0)
		{
			LongRangePatterns.Emplace(Vec4);
			Count++;
		}
		if (Vec5.Y == 0)
		{
			LongRangePatterns.Emplace(Vec5);
			Count++;
		}
		if (Vec6.Y == 0)
		{
			LongRangePatterns.Emplace(Vec6);
			Count++;
		}
		if (Vec7.Y == 0)
		{
			LongRangePatterns.Emplace(Vec7);
			Count++;
		}

		if (Count == 0)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_ATTACKPATTERN3, FVector(0, false, true));
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_ATTACKPATTERN5_1, FVector(1, false, true));
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_ATTACKPATTERN5_2, FVector(2, false, true));
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_ATTACKPATTERN5_3, FVector(3, false, true));
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_ATTACKPATTERN6, FVector(4, false, true));
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_ATTACKPATTERN8, FVector(5, false, true));
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_ATTACKPATTERN9, FVector(6, false, true));

			LongRangePatterns.Emplace(OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN3));
			LongRangePatterns.Emplace(OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN5_1));
			LongRangePatterns.Emplace(OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN5_2));
			LongRangePatterns.Emplace(OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN5_3));
			LongRangePatterns.Emplace(OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN6));
			LongRangePatterns.Emplace(OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN8));
			LongRangePatterns.Emplace(OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_ATTACKPATTERN9));

			int32 Index = FMath::RandRange(0, 6);
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(BBKEY_ATTACKPATTERNSELECTED, LongRangePatterns[Index].X);
		}
		else
		{
			int32 Index = FMath::RandRange(0, Count - 1);
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(BBKEY_ATTACKPATTERNSELECTED, LongRangePatterns[Index].X);
		}
	}

	bResult = bInShortRange;
	return bResult;
}
