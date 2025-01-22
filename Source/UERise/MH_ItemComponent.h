// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"

#include "MH_ItemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickup, AActor*, Inventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemDrop);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemUsed, AActor*, User, FGameplayTag, GameplayTag);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UERISE_API UMH_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMH_ItemComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
    void PickUpItem(AActor* InventoryPicking);

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
    void DropItem();

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
    void UseItem(AActor* User, FGameplayTag GameplayTag);

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
    bool SplitItems(const int32 SplitSize);

    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
    void DestroyItem();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;


public:

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category = "Identification")
    FGuid ItemId;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category = "Identification")
    FGuid InventoryId;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category = "Identification")
    FGuid OwnerId;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Quantity")
    bool bStackable = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "Quantity")
    int32 MaxStack = 99;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category = "Quantity")
    int32 CurrentStack = 1;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, SaveGame, Category = "BaseInfo")
    FName ItemName;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "BaseInfo")
    bool bSimulateWhenDropped = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated, Category = "BaseInfo")
    FGameplayTag ItemTagSlotType;

    UPROPERTY(BlueprintAssignable, EditAnywhere, Category = "Events")
    FOnPickup OnPickup;

    UPROPERTY(BlueprintAssignable, EditAnywhere, Category = "Events")
    FOnItemDrop OnItemDropped;

    UPROPERTY(BlueprintAssignable, EditAnywhere, Category = "Events")
    FOnItemUsed OnItemUsed;
		
};
