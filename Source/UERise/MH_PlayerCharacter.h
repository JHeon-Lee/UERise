// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "GameData/MHGlobalEnum.h"
#include "Interface/MH_AnimNotifyInterface.h"
#include "Interface/MHWidgetInterface.h"
#include "Interface/MH_WeaponComponentInterface.h"
#include "Components/TimelineComponent.h"
#include "MH_PlayerCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWheelTriggered, float /*ChagedValue*/);
DECLARE_MULTICAST_DELEGATE(FOnXButtonTriggered);


UCLASS()
class UERISE_API AMH_PlayerCharacter : public ACharacter, public IMH_AnimNotifyInterface, public IMHWidgetInterface, public IMH_WeaponComponentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMH_PlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:
	void ComponentAttach();
	void Initialize();
	void PlayUseItemMontage();
	
	UFUNCTION()
	void GetItemNearby();

	UFUNCTION()
	void GetItemCallBack(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);




public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:
	void URotate(const FInputActionValue& Value);
	void ULook(const FInputActionValue& Value);
	void ChangeItemSlot(const FInputActionValue& Value);

	// 버튼으로 시작되는 몽타주 재생
	void ComboStartA();
	void ComboStartY();
	void ComboStartRT();
	void ComboStartB();

	//Bool On/Off
	void PressAOff() { PressA = false; }
	void PressBOff() { PressB = false; }
	void PressXOff() { PressX = false; }
	void PressYOff() { PressY = false; }
	void PressRBOff() { PressRB = false; }
	void PressRTOff() { PressRT = false; }
	void PressLTOff() { PressLT = false; }
	void PressWASDOff() { PressWASD = false; ActionValue = FVector2D(0.0, 0.0); KeyDirInt = 0;}

	void PressBOn() { PressB = true; }
	void PressXOn();
	void PressYOn() { PressY = true; }
	void PressRBOn() { PressRB = true; }
	void PressRTOn() { PressRT = true; }
	void PressLTOn() { PressLT = true; }

	void SwitchAtkMode();


public:
// Called every frame
	virtual void Tick(float DeltaTime) override;

	void KeyPressCheck();
	void MakeFalling();

	void PlayValutMontage();


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//DamageTaken
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void StartHitStop(float Time);
	void EndHitStop() { CustomTimeDilation = 1.0f; }


//AnimNotify Interface Override

	virtual void CameraShake(bool IsStrong) override;

	virtual void TurnOnBuffEffect() override;	

	virtual void ManualMoveBegin() override;
	virtual void ManualMoveTick(float ManualMoveSpeed, float ManualMoveZSpeed, float FlymodeTime, bool WallRun) override;
	virtual void ManualMoveEnd() override;

	virtual void ShootWirebugBegin(FName SocketName) override;
	virtual void ShootWirebugTick(float Distance, float ZDistance, float FDistance, float Movetime) override;
	virtual void ShootWirebugEnd() override;

	virtual void ComboTick(TMap<EKeyInfo, TObjectPtr<class UAnimMontage>> MontageMap, FName SectionName) override;
	virtual void ComboEnd(bool IsChargeAtk) override;

	virtual void AttackBegin() override;
	virtual void AttackTick(FName AtkStartSocket, FName AtkEndSocket, float AtkRadius) override;

	virtual void DamageTakeBegin(EDamageTakeType DmgTakeType) override;
	virtual void DamageTakeEnd() override;

	virtual void ValutBegin(float Offset) override;
	virtual void ValutEnd() override;

	virtual void RotateTick(float InitialYaw, float MaxRotateDegree, float RotateSpeed) override;

	virtual void UseItemBegin() override;
	virtual void UseItemTick() override;
	virtual void UseItemEnd() override;

// Widget Interface Override
	virtual void SetupCharacterHPWidget(class UMHUserWidget* InUserWidget) override;
	virtual void SetupItemWidget(class UMHUserWidget* InUserWidget) override;
	virtual void SetupHUD(class UMHHUDWidget* MHHUD) override;

// Weapon Interface Override
	virtual void SetPlayerState(EWeaponType Type) override;



//
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ChargeStopped();

	UFUNCTION(BlueprintCallable)
	void AttachGreatSwdComponent();


protected:
	// Component Attach
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<class UMH_PlayerStatComponent> StatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TObjectPtr<class UMHWidgetComponent> PlayerWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Valut")
	TObjectPtr<class UMHValutComponent> PlayerValutComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class UMHEquipmentComponent> EquipmentComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> WireBug;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UNiagaraComponent> BuffEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UParticleSystemComponent> BugEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Item)
	TObjectPtr <class UMH_ItemManager> Inventory;

	//Reference
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref")
	TObjectPtr<class APostProcessVolume> PostProcessVolumeRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref")
	TObjectPtr<class UMaterialInstanceDynamic> PostProcessMaterialRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref")
	TObjectPtr<class UMaterialInstance> OverlayMaterialInstanceRef;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> OverlayMaterialDynamicInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref")
	TObjectPtr<class APawn> NearPawnRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref")
	TObjectPtr<class AActor> NearPropRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref")
	TObjectPtr<class UDataTable> GSwdAttackDataTableRef;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TObjectPtr<class UNiagaraSystem> HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TObjectPtr<class UNiagaraSystem> CriticalHitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref")
	TSubclassOf<class ULegacyCameraShake> CamShakeWeak;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref")
	TSubclassOf<class ULegacyCameraShake> CamShakeStrong;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ref")
	TObjectPtr<UCurveFloat> OpacityCurve;

	//Timeline
	FTimeline OpacityFloatTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Key")
	TMap<EButtons, bool> PressMap;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	bool PressWASD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	bool PressA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	bool PressB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	bool PressX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	bool PressY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	bool PressRB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	bool PressRT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	bool PressLT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	int32 KeyDirInt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	float KeyDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	TMap<EKeyInfo, bool> KeyArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool AtkOncePerMonta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool Busy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool ToOppositeDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool ManualMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool WallRunning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool NeedZVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float InitialZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FVector InitialPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CurrentTrgtZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int32 ChargeStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FVector2D ActionValue;

	//----------------------------------Stat---------------------------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHpModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurrentHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxStaminaModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxWeaponDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurrentWeaponDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Atk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float AtkModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Critical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 WireBugStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	uint8 CurWireBugStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float ANSTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float BuffTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
	ELevelType LevelType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
	EDamageTakeType DamageTakeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TMap<FName,TObjectPtr<class UAnimMontage>> ComboStartMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TMap<FName, TObjectPtr<class UAnimMontage>> PropGetMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TMap<EValutMontage, TObjectPtr<class UAnimMontage>> VaultMontages;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TArray<TObjectPtr<class UAnimMontage>> RollMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TArray<TObjectPtr<class UAnimMontage>> SpecialMoveMontages;

	UPROPERTY()
	TObjectPtr<class UMH_ItemComponent> NearbyItem;


// Input
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> UtusiMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_A;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_B;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_RB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_RT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_LT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Look;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Wheel;


	FOnWheelTriggered OnWheelTrigged;

	FOnXButtonTriggered OnXButtonTriggered;


};
