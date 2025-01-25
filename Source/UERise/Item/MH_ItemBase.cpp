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
	ItemWidgetComponent->SetVisibility(false);

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereCollisionComponent->InitSphereRadius(200.0f);
	SphereCollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMH_ItemBase::OnSphereBeginOverlap);
	SphereCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AMH_ItemBase::OnSphereEndOverlap);
}


void AMH_ItemBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ItemWidgetComponent->SetVisibility(true);
}

void AMH_ItemBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("endOverlap"));
	ItemWidgetComponent->SetVisibility(false);
}
