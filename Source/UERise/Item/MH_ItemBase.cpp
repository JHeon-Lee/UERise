// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/MH_ItemBase.h"
#include "Components/WidgetComponent.h"
#include "Item/MH_ItemComponent.h"

AMH_ItemBase::AMH_ItemBase()
{
	RootComponent = GetStaticMeshComponent();

	ItemComponent = CreateDefaultSubobject<UMH_ItemComponent>(TEXT("ItemComponent"));

	// WidgetComponent
	ItemWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	ItemWidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereCollisionComponent->InitSphereRadius(200.0f);
	SphereCollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereCollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	

	SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMH_ItemBase::OnSphereBeginOverlap);
	
}

void AMH_ItemBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UMH_ItemManager* ActorInventory = FindComponentByClass<UMH_ItemManager>())
	{
		if (IsValid(ActorInventory))
		{
			ItemComponent->PickUpItem(ActorInventory);
		}		
	}	
}
