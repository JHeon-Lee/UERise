// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "MH_ItemManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUpdated, AActor*, Item);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UERISE_API UMH_ItemManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMH_ItemManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    void SetupInventoryStorageReference();

    bool bHasPlayerState = false;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
        
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "MH_ITEM")
    bool AddItemsOfClass(const TSubclassOf<AActor> Class, const int32 Quantity);
        
    UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "MH_ITEM")
    bool RemoveItemsOfClass(const TSubclassOf<AActor> Class, const int32 Quantity);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MH_ITEM")
    TArray<AActor*> GetAllItems();

    UFUNCTION(BlueprintCallable, Category = "MH_ITEM")
    bool GetAllItemsOfClass(const TSubclassOf<AActor> Class, TArray<AActor*>& OutFilteredArray);
        
    UFUNCTION(BlueprintCallable, Category = "MH_ITEM")
    bool HasEnoughItems(const TSubclassOf<AActor> Item,const int32 Quantity);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "MH_ITEM")
    bool HasExactItem(AActor* Item);


public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, SaveGame)
    FGuid InventoryId;

    UPROPERTY(BlueprintReadWrite, Replicated)
    AActor* InventoryStorage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bDebug;

    // Called whenever an item is updated inside the inventory.
    UPROPERTY(BlueprintAssignable)
    FOnItemUpdated OnItemUpdated;
};
