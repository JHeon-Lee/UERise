// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/MHRathalosAIInterface.h"
#include "Interface/MHRathalosGetterInterface.h"
#include "Interface/MHRathalosSetterInterface.h"
#include "Interface/MHMonsterAttackInterface.h"
#include "GameData/MHMonsterStat.h"
#include "MHCharacterRathalos.generated.h"

UCLASS()
class UERISE_API AMHCharacterRathalos : public ACharacter, public IMHRathalosAIInterface, public IMHRathalosGetterInterface, public IMHMonsterAttackInterface, public IMHRathalosSetterInterface

{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMHCharacterRathalos();

	virtual void PostInitializeComponents() override;

	// Attack Hit Section
protected:
	virtual bool AttackHitCheck1() override;
	virtual bool AttackHitCheck2() override;
	virtual bool AttackHitCheck3() override;
	virtual bool AttackHitCheck4() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventIntigator, AActor* DamageCauser) override;

	// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AirDeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> CaptureMontage;

	void SetDead();
	void SetCaptured();

	// Damaged Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AirHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> CriticalHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AirCriticalHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> KnockDownMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AirKnockDownMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> TailCutMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AirTailCutMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> StunTrapMontage;

	void SetHeadBreak();
	void SetLegBreak();
	void SetTailCut();
	void SetWingBreak();
	void SetStun();

	UFUNCTION(BlueprintCallable)
	void SetStunTrapEffected();

	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMHRathalosStatComponent> Stat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	bool bIsAnger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	bool bHasAngered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	bool bIsTailCut;

	// Montage Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> RotateLeft90Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> RotateLeft180Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> RotateRight90Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> RotateRight180Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> Rotate2Left90Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> Rotate2Left180Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> Rotate2Right90Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> Rotate2Right180Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> NoDetectMoveForwardMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DetectMoveForwardMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> RoarMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackPattern1Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackPattern2Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackPattern3Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackPattern4Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackPattern5Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackPattern5_1Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackPattern5_2Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackPattern6Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackPattern7Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackPattern8Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackPattern8_1Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackPattern9Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> Pattern3RotateLeft90Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> Pattern3RotateLeft180Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> Pattern3RotateRight90Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> Pattern3RotateRight180Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> FlyMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> FlyRotateLeft90Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> FlyRotateLeft180Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> FlyRotateRight90Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> FlyRotateRight180Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AngerRoarMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> EscapeRoarMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> DragonRideReadyMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> TurfWarReadyMontage;

	void AttackEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void RotateEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void Rotate2End(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void MoveEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void RoarEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void Pattern3RotateEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void FlyEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void FlyRotateEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void RunAI(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void EscapeRotateEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void EscapeMoveEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	// AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackShortRange() override;
	virtual float GetAIAttackLongRange() override;
	virtual float GetAILostTargetDistance() override;

	virtual void SetAIAttackDelegate(const FAIRathalosAttackFinished& InOnAttackFinished) override;
	virtual void SetAIRotateDelegate(const FAIRathalosRotateFinished& InOnRotateFinished) override;
	virtual void SetAIRotate2Delegate(const FAIRathalosRotate2Finished& InOnRotate2Finished) override;
	virtual void SetAIMoveDelegate(const FAIRathalosMoveFinished& InOnMoveFinished) override;
	virtual void SetAIRoarDelegate(const FAIRathalosRoarFinished& InOnRoarFinished) override;
	virtual void SetAIFlyDelegate(const FAIRathalosFlyFinished& InOnFlyFinished) override;
	virtual void SetAIFlyRotateDelegate(const FAIRathalosFlyRotateFinished& InOnFlyRotateFinished) override;
	virtual void SetAIEscapeRotateDelegate(const FAIRathalosEscapeRotateFinished& InOnEscapeRotateFinished) override;
	virtual void SetAIEscapeMoveDelegate(const FAIRathalosEscapeMoveFinished& InOnEscapeMoveFinished) override;

	virtual void AttackPattern1AI() override;
	virtual void AttackPattern2AI() override;
	virtual void AttackPattern3AI() override;
	virtual void AttackPattern4AI() override;
	virtual void AttackPattern5AI() override;
	virtual void AttackPattern5_1AI() override;
	virtual void AttackPattern5_2AI() override;
	virtual void AttackPattern6AI() override;
	virtual void AttackPattern7AI() override;
	virtual void AttackPattern8AI() override;
	virtual void AttackPattern8_1AI() override;
	virtual void AttackPattern9AI() override;

	virtual void FlyAI() override;
	virtual void FlyRotateLeft90AI(float InAngleRate) override;
	virtual void FlyRotateLeft180AI(float InAngleRate) override;
	virtual void FlyRotateRight90AI(float InAngleRate) override;
	virtual void FlyRotateRight180AI(float InAngleRate) override;

	virtual void RotateLeft90AI(float InAngleRate) override;
	virtual void RotateLeft180AI(float InAngleRate) override;
	virtual void RotateRight90AI(float InAngleRate) override;
	virtual void RotateRight180AI(float InAngleRate) override;

	virtual void Rotate2Left90AI(float InAngleRate) override;
	virtual void Rotate2Left180AI(float InAngleRate) override;
	virtual void Rotate2Right90AI(float InAngleRate) override;
	virtual void Rotate2Right180AI(float InAngleRate) override;

	virtual void EscapeRotateLeft90AI(float InAngleRate) override;
	virtual void EscapeRotateLeft180AI(float InAngleRate) override;
	virtual void EscapeRotateRight90AI(float InAngleRate) override;
	virtual void EscapeRotateRight180AI(float InAngleRate) override;
	virtual void EscapeMoveForwardAI() override;

	virtual void NoDetectMoveForwardAI(float InDistanceRate) override;
	virtual void DetectMoveForwardAI(float InDistanceRate) override;

	virtual void RoarAI() override;
	virtual void AngerRoarAI() override;
	virtual void EscapeRoarAI() override;

	virtual void DragonRideReadyAI() override;
	virtual void TurfWarReadyAI() override;

	virtual void StopMontage() override;

	virtual bool IsHp75AI() override;

	FAIRathalosAttackFinished OnAttackFinished;
	FAIRathalosRotateFinished OnRotateFinished;
	FAIRathalosRotate2Finished OnRotate2Finished;
	FAIRathalosMoveFinished OnMoveFinished;
	FAIRathalosRoarFinished OnRoarFinished;
	FAIRathalosFlyFinished OnFlyFinished;
	FAIRathalosFlyRotateFinished OnFlyRotateFinished;
	FAIRathalosEscapeRotateFinished OnEscapeRotateFinished;
	FAIRathalosEscapeMoveFinished OnEscapeMoveFinished;

	// AnimNotify Section
protected:
	virtual void SetMovementModeWalking() override;
	virtual void SetNoCollision() override;

	// Getter Section
protected:
	virtual bool GetIsFlying() override;

	// Escape Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	bool b270secElapsed;

	virtual bool Get270secElapsedAI() override;

	// Dragon Riding Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsDragonRiding : 1;

	virtual bool GetIsDragonRiding() override;
};
