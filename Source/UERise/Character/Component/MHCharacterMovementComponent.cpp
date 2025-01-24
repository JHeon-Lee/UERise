// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/MHCharacterMovementComponent.h"

void UMHCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
    Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

    EMovementMode CurrentMode = MovementMode;    

    // MovementMode Change
    switch (CurrentMode)
    {
    case MOVE_Walking: 
        OnMovementWalkingDelegate.Broadcast();
        break;
    case MOVE_Falling:
        OnMovementFallingDelegate.Broadcast();
        break;
    default:
        break;
    }

    PrevMovementMode = CurrentMode;
}
