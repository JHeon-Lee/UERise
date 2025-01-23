// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Item/MH_ItemManager.h"

#include "MH_ItemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickup, UMH_ItemManager*, Inventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemUsed, AActor*, User, FGameplayTag, GameplayTag);


UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UERISE_API UMH_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

    friend UMH_ItemManager;

public:	
	// Sets default values for this component's properties
	UMH_ItemComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
    void PickUpItem(UMH_ItemManager* InventoryPick);

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
    void UseItem(AActor* User, FGameplayTag GameplayTag);

protected:
    // Called when the game starts
    virtual void BeginPlay() override;


public:
    static const FName TAG_ITEM;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category = "Identification")
    FGuid ItemId;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category = "Identification")
    FGuid InventoryId;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category = "Identification")
    FGuid OwnerId;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Quantity")
    int32 MaxStack = 99;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category = "Quantity")
    int32 CurrentStack = 1;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category = "BaseInfo")
    FName ItemName;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "BaseInfo")
    FGameplayTag ItemTagSlotType;

    UPROPERTY(BlueprintAssignable, EditAnywhere, Category = "Events")
    FOnPickup OnPickup;

    UPROPERTY(BlueprintAssignable, EditAnywhere, Category = "Events")
    FOnItemUsed OnItemUsed;
		
};
