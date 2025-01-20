// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/MHCharacterMovementComponent.h"

void UMHCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
    Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

    EMovementMode CurrentMode = MovementMode;    

    UE_LOG(LogTemp, Log, TEXT("Movement mode changed: %d -> %d"), PrevMovementMode, CurrentMode);

    // MovementMode Change
    switch (CurrentMode)
    {
    case MOVE_Walking:
        UE_LOG(LogTemp, Log, TEXT("Now Walking"));                
        OnMovementWalkingDelegate.Broadcast();
        break;
    case MOVE_Falling:
        UE_LOG(LogTemp, Log, TEXT("Now Falling"));
        OnMovementFallingDelegate.Broadcast();
        break;
    default:
        break;
    }

    PrevMovementMode = CurrentMode;
}
