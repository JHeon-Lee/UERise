// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/MH_GswdNotifyInterface.h"
#include "Components/TimelineComponent.h"
#include "GameData/MHGlobalEnum.h"
#include "MHGreatSwordComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMontagePlayDelegate, UAnimMontage*);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UERISE_API UMHGreatSwordComponent : public UActorComponent, public IMH_GswdNotifyInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMHGreatSwordComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void Deactivate() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void SwdAttachToSocket(FName socketName) override;

	virtual void GSwdFstCharge() override;
	virtual void GSwdSndCharge() override;
	virtual void GSwdTrdCharge() override;

	virtual void TurnOnBuffEffect() override;

	void StopCharge();
	void StopChargeCallback();

	void ComboStartA();
	void ComboStartY(TMap<EButtons, bool> KeyInfo, EWeaponType WeaponState, bool IsWireBugEnough);


	UFUNCTION()
	void OpacityUpdate(float Opcity);

public:
	FOnMontagePlayDelegate MontagePlayDelegete;

private:
	UPROPERTY()
	TObjectPtr<class USkeletalMeshComponent> OwnerMesh;

	UPROPERTY()
	TObjectPtr<class UAnimInstance> OwnerAnimInstance;

	FTimeline OpacityFloatTimeline;



protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> Gswd;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UParticleSystemComponent> SwdFlameEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UNiagaraComponent> BodyFlameEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UCurveFloat> OpacityCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref")
	TObjectPtr<class UMaterialInstance> OverlayMaterialInstanceRef;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> OverlayMaterialDynamicInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TMap<FName, TObjectPtr<class UAnimMontage>> ComboStartMontage;
};
