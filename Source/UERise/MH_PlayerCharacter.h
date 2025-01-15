// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Interface/MH_AnimNotifyInterface.h"
#include "Interface/MHWidgetInterface.h"
#include "Components/TimelineComponent.h"
#include "MH_PlayerCharacter.generated.h"


//FOnTimelineFloat GSwdOverlayMaterialOpacity;


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Unarmed UMETA(DisplayName = "Unarmed"),
	Armed UMETA(DisplayName = "Armed"),
	Riding UMETA(DisplayName = "Riding"),
	DragonRide UMETA(DisplayName = "DragonRide")
};

UENUM(BlueprintType)
enum class ELevelType : uint8
{
	Village UMETA(DisplayName = "Village"),
	Field UMETA(DisplayName = "Field")
};

UENUM(BlueprintType)
enum class EValutMontage : uint8
{
	WallRun UMETA(DisplayName = "WallRun"),
	JumpOver UMETA(DisplayName = "JumpOver"),
	Vault UMETA(DisplayName = "Vault")
};

UENUM(BlueprintType)
enum class EDamageTakeType : uint8
{
	Default UMETA(DisplayName = "Default"),
	Evade UMETA(DisplayName = "Evade"),
	Gaurd UMETA(DisplayName = "Gaurd"),
	Tackle UMETA(DisplayName = "Tackle"),
};

UCLASS()
class UERISE_API AMH_PlayerCharacter : public ACharacter, public IMH_AnimNotifyInterface, public IMHWidgetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMH_PlayerCharacter();

private:
	void ComponentAttach();
	void InputSystemSetting();


public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:
	void URotate(const FInputActionValue& Value);
	void ULook(const FInputActionValue& Value);

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
	void PressWASDOff() { PressWASD = false; ActionValue = FVector2D(0.0, 0.0); }

	void PressBOn() { PressB = true; }
	void PressXOn() { PressX = true; }
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

	// Valut
	void ValutCheck();
	void GetFrontObjectLocation();
	void GetObjectDimension();
	void PlayValutMontatge();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//DamageTaken
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void StartHitStop(float Time);
	void EndHitStop() { CustomTimeDilation = 1.0f; }
	void StopCharge();
	void StopChargeCallback();


//AnimNotify Interface Override
	virtual void SwdAttachToSocket(FName socketName) override;

	virtual void CameraShake(bool IsStrong) override;

	virtual void GSwdFstCharge() override;
	virtual void GSwdSndCharge() override;
	virtual void GSwdTrdCharge() override;

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

// Widget Interface Override
	virtual void SetupCharacterWidget(class UMHUserWidget* InUserWidget) override;

	UFUNCTION()
	void OpacityUpdate(float Opcity);

//
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ChargeStopped();


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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<class UMH_PlayerStatComponent> StatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TObjectPtr<class UMHWidgetComponent> PlayerWidgetComponent;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	float TargetRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	float DistanceToWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	float VaultOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	float WallSlope;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	float WallRotZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	FVector ImpactPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	FVector InitialImpactPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	FVector WarpPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall Check")
	FVector ImpactNormal;

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
	TArray<bool> KeyArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool AtkOncePerMonta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool Busy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool ToOppositeDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool CanWallRun;

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

protected:

// Component Attach
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> WireBug;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class USkeletalMeshComponent> Gswd;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UNiagaraComponent> BodyFlameEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UNiagaraComponent> BuffEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UParticleSystemComponent> BugEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UParticleSystemComponent> SwdFlameEffect;


// Input
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> UtusiMappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_A;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_B;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_X;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Y;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_RB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_RT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_LT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Move;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> IA_Look;	


};
