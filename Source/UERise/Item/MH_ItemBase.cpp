// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/MH_ItemBase.h"
#include "Components/WidgetComponent.h"
#include "Item/MH_ItemComponent.h"

AMH_ItemBase::AMH_ItemBase()
{
	RootComponent = GetStaticMeshComponent();
	GetStaticMeshComponent()->SetCollisionProfileName(TEXT("Custom"), true);	
	GetStaticMeshComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);


	ItemComponent = CreateDefaultSubobject<UMH_ItemComponent>(TEXT("ItemComponent"));
	ItemComponent->OnItemUsed.AddDynamic(this, &AMH_ItemBase::UseItem);

	// WidgetComponent
	ItemWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	ItemWidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	ItemWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereCollisionComponent->InitSphereRadius(200.0f);
	SphereCollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMH_ItemBase::OnSphereBeginOverlap);
	SphereCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AMH_ItemBase::OnSphereEndOverlap);
}

void AMH_ItemBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("BeginOverlap"));
	ItemWidgetComponent->SetVisibility(true);
}

void AMH_ItemBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("endOverlap"));
	ItemWidgetComponent->SetVisibility(false);
}

void AMH_ItemBase::UseItem
(AActor* Item, FGameplayTag ItemTag)
{
	UE_LOG(LogTemp, Log, TEXT("UseItem Called : %s"), *FString(Item->GetName()));

}
