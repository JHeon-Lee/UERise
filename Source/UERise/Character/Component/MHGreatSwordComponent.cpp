// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Component/MHGreatSwordComponent.h"
#include "Camera/CameraComponent.h"
#include "Interface/MH_WeaponComponentInterface.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Materials/MaterialInterface.h"
#include "GameData/MHGlobalEnum.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "MHGameInstance.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Engine/PostProcessVolume.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values for this component's properties
UMHGreatSwordComponent::UMHGreatSwordComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Gswd = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gswd"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/armorTest/Wyvern/G_Swd086_meshout.G_Swd086_meshout'"));
	if (WeaponMeshRef.Object)
	{
		Gswd->SetSkeletalMesh( WeaponMeshRef.Object);
		Gswd->SetRelativeRotation(FRotator(0, 0, 90));
	}

	SwdFlameEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SwdFlameEffect"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FlameVFXRef(TEXT("/Script/Engine.ParticleSystem'/Game/MarketContent/Realistic_Starter_VFX_Pack_Vol2/Particles/Fire/NewP_flame.NewP_flame'"));
	if (FlameVFXRef.Object)
	{
		SwdFlameEffect->SetTemplate(FlameVFXRef.Object);
		SwdFlameEffect->SetRelativeLocation(FVector(0, 0, 160));
		SwdFlameEffect->SetRelativeRotation(FRotator(-90, 0, 0));
		SwdFlameEffect->SetAutoActivate(false);
	}	

	BodyFlameEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BodyFlameEffect"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> BodyVFXRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/MarketContent/KiAura/SpritesAuras/NS_AuraSkeleton1.NS_AuraSkeleton1'"));
	if (BodyVFXRef.Object)
	{
		BodyFlameEffect->SetAsset(BodyVFXRef.Object);
		BodyFlameEffect->SetAutoActivate(false);
	}	
	
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFloat(TEXT("/Script/Engine.CurveFloat'/Game/Characters/Utusi/Data/OpacityFloat.OpacityFloat'"));
	if (CurveFloat.Object)
	{
		OpacityCurve = CurveFloat.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialInstance(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/armorTest/BodyGlareMatInst.BodyGlareMatInst'"));
	if(MaterialInstance.Object)
	{
		OverlayMaterialInstanceRef = MaterialInstance.Object;
	}

	if (OverlayMaterialInstanceRef)
	{
		OverlayMaterialDynamicInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), OverlayMaterialInstanceRef);
		Gswd->SetOverlayMaterial(OverlayMaterialDynamicInstance);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> PPMaterialInstance(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/PostProcessMat/RadicalBlur_Inst.RadicalBlur_Inst'"));
	if (PPMaterialInstance.Object)
	{
		PostProcessMaterialInstanceRef = PPMaterialInstance.Object;
	}

	FOnTimelineFloat GSwdOverlayMaterialOpacity;
	GSwdOverlayMaterialOpacity.BindUFunction(this, FName("OpacityUpdate"));
	OpacityFloatTimeline.AddInterpFloat(OpacityCurve, GSwdOverlayMaterialOpacity);


}


// Called when the game starts
void UMHGreatSwordComponent::BeginPlay()
{
	Super::BeginPlay();	

	UE_LOG(LogTemp, Log, TEXT("GreatSwd Activated!"));

	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		check(OwnerCharacter->GetMesh());
		OwnerMesh = OwnerCharacter->GetMesh();
		OwnerAnimInstance = OwnerMesh->GetAnimInstance();
	}

	Gswd->AttachToComponent(OwnerMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false), TEXT("B_Weapon"));

	BodyFlameEffect->SetupAttachment(OwnerMesh);

	SwdFlameEffect->AttachToComponent(Gswd, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	FOnTimelineFloat FOVTimelineFloat;
	FOVTimelineFloat.BindUFunction(this, FName("FOVUpdate"));
	UCurveFloat* FOVCurve = Cast<UMHGameInstance>(GetWorld()->GetGameInstance())->FindCurveFloat("GameData.Curve.GSwdFOV");
	if (FOVCurve)
	{
		FOVTimeline.AddInterpFloat(FOVCurve, FOVTimelineFloat);
	}

	FOnTimelineFloat ZoomTimelineFloat;
	ZoomTimelineFloat.BindUFunction(this, FName("ZoomUpdate"));
	UCurveFloat* ZoomCurve = Cast<UMHGameInstance>(GetWorld()->GetGameInstance())->FindCurveFloat("GameData.Curve.GSwdZoom");
	if (ZoomCurve)
	{
		ZoomTimeline.AddInterpFloat(ZoomCurve, ZoomTimelineFloat);
	}

	FOnTimelineFloat BlendWeightTimelineFloat;
	BlendWeightTimelineFloat.BindUFunction(this, FName("BlendWeightUpdate"));
	UCurveFloat* BlendWeightCurve = Cast<UMHGameInstance>(GetWorld()->GetGameInstance())->FindCurveFloat("GameData.Curve.GSwdBlendWeight");
	if (BlendWeightCurve)
	{
		BlendWeightTimeline.AddInterpFloat(BlendWeightCurve, BlendWeightTimelineFloat);
	}

	// Find PostProcess Ref 
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APostProcessVolume::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		APostProcessVolume* PPVolume = Cast<APostProcessVolume>(Actor);
		if (PPVolume && PPVolume->bUnbound)
		{			
			if (PPVolume->Settings.WeightedBlendables.Array.Num() <= 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Found Unbound PostProcessVolume, But WeightedBlendables Array Not Exist"));
				break;
			}

			if (!PostProcessMaterialInstanceRef)
			{
				UE_LOG(LogTemp, Warning, TEXT("PostProcessMaterialInstanceRef is NULL"));
				break;
			}

			PostProcessMaterialDynamicInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), PostProcessMaterialInstanceRef);
			//PostProcessMaterialDynamicInstance->SetScalarParameterValue(TEXT("Alpha"), 0.0);
			// Apply Setting to PostProcessVolume
			PPVolume->Settings.WeightedBlendables.Array[0].Object = PostProcessMaterialDynamicInstance;		

			PostProcessVolumeRef = PPVolume;			
			PostProcessVolumeRef->Settings.WeightedBlendables.Array[0].Weight = 0.0;
		}
	}
}

void UMHGreatSwordComponent::Deactivate()
{
	Super::Deactivate();

	UE_LOG(LogTemp, Log, TEXT("GreatSwd Destroyed!"));

	DestroyComponent(true);
	Gswd->DestroyComponent();
	BodyFlameEffect->DestroyComponent();
	SwdFlameEffect->DestroyComponent();

	OwnerMesh = nullptr;
}


// Called every frame
void UMHGreatSwordComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	OpacityFloatTimeline.TickTimeline(DeltaTime);
	FOVTimeline.TickTimeline(DeltaTime);
	ZoomTimeline.TickTimeline(DeltaTime);
	BlendWeightTimeline.TickTimeline(DeltaTime);
}

void UMHGreatSwordComponent::SwdAttachToSocket(FName socketName)
{	
	check(OwnerMesh);

	if (OwnerMesh->DoesSocketExist(socketName))
	{
		Gswd->AttachToComponent(OwnerMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), socketName);
		IMH_WeaponComponentInterface* CharacterPlayer = Cast<IMH_WeaponComponentInterface>(GetOwner());
		if (CharacterPlayer)
		{
			if (socketName == TEXT("B_Weapon"))
			{
				CharacterPlayer->SetPlayerState(EWeaponType::Unarmed);
			}
			else if (socketName == TEXT("L_Weapon"))
			{
				CharacterPlayer->SetPlayerState(EWeaponType::GreatSwdArmed);
			}
		}
	}
}

void UMHGreatSwordComponent::OpacityUpdate(float Opcity)
{
	OverlayMaterialDynamicInstance->SetScalarParameterValue(TEXT("Opacity"), Opcity);
}

void UMHGreatSwordComponent::GSwdFstCharge()
{
	OpacityFloatTimeline.PlayFromStart();
	OverlayMaterialDynamicInstance->SetVectorParameterValue(TEXT("Color"), FVector4(50.0, 8.0, 8.0));
}

void UMHGreatSwordComponent::GSwdSndCharge()
{
	OpacityFloatTimeline.PlayFromStart();
	OverlayMaterialDynamicInstance->SetVectorParameterValue(TEXT("Color"), FVector4(50.0, 8.0, 1.3));
}

void UMHGreatSwordComponent::GSwdTrdCharge()
{
	OpacityFloatTimeline.PlayFromStart();
	OverlayMaterialDynamicInstance->SetVectorParameterValue(TEXT("Color"), FVector4(19.5, 50.0, 50.0));

	BodyFlameEffect->SetActive(true);
	SwdFlameEffect->SetActive(true);

	// ChargeEndVFX
	UNiagaraSystem* ChargeEndVFX = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Script/Niagara.NiagaraSystem'/Game/MarketContent/SlashHitAndStabHit/Particles/Niagara/NS_ky_stabHit08.NS_ky_stabHit08'"));
	if (ChargeEndVFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(ChargeEndVFX, Gswd, TEXT("None"), FVector(0, 0, 0), FRotator(0, 0, 0), EAttachLocation::KeepRelativeOffset, true);
	}
}

void UMHGreatSwordComponent::TurnOnBuffEffect()
{

}

void UMHGreatSwordComponent::PostProcessEffectOn()
{
	if (PostProcessVolumeRef)
	{
		PostProcessVolumeRef->Settings.WeightedBlendables.Array[0].Weight = 1.0f;
		FOVTimeline.Play();
		ZoomTimeline.Play();
		BlendWeightTimeline.Play();
	}
}

void UMHGreatSwordComponent::PostProcessEffectOff()
{	
	if (PostProcessVolumeRef)
	{
		PostProcessVolumeRef->Settings.WeightedBlendables.Array[0].Weight = 0.0f;
		FOVTimeline.Reverse();
		ZoomTimeline.Reverse();
		BlendWeightTimeline.Reverse();
	}

}

void UMHGreatSwordComponent::StopCharge()
{
	FTimerHandle TimeHandle;
	GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &UMHGreatSwordComponent::StopChargeCallback, 2.0, false);
}

void UMHGreatSwordComponent::StopChargeCallback()
{
	SwdFlameEffect->SetActive(false);
	BodyFlameEffect->SetActive(false);
	OverlayMaterialDynamicInstance->SetScalarParameterValue(TEXT("Opacity"), 0.0);
}

void UMHGreatSwordComponent::ComboStartA()
{
	OwnerAnimInstance->Montage_Play(FIND_MONTAGE("Character.Player.Montage.A_GSwd"));
}

void UMHGreatSwordComponent::ComboStartY(TMap<EButtons, bool> KeyInfo, EWeaponType WeaponState, bool IsWireBugEnough)
{
	if (WeaponState == EWeaponType::GreatSwdArmed)
	{
		if (KeyInfo[EButtons::B])
		{
			OwnerAnimInstance->Montage_Play(FIND_MONTAGE("Character.Player.Montage.Y+B_GSwd"));
		}
		else if (KeyInfo[EButtons::LT] && IsWireBugEnough)
		{
			OwnerAnimInstance->Montage_Play(FIND_MONTAGE("Character.Player.Montage.Y+LT_GSwd"));
		}
		else if (!KeyInfo[EButtons::LT])
		{
			OwnerAnimInstance->Montage_Play(FIND_MONTAGE("Character.Player.Montage.Y_GSwd"));
		}
	}
	else if (WeaponState == EWeaponType::Unarmed)
	{
		if (!KeyInfo[EButtons::LT] && KeyInfo[EButtons::RStick])
		{
			OwnerAnimInstance->Montage_Play(FIND_MONTAGE("Character.Player.Montage.Y+Move_Unarmed"));
		}

	}
}


void UMHGreatSwordComponent::FOVUpdate(float FOVValue)
{	
	UCameraComponent* OwnerCamera = GetOwner()->FindComponentByClass<UCameraComponent>();
	if (OwnerCamera)
	{
		OwnerCamera->SetFieldOfView(FOVValue);
	}
}

void UMHGreatSwordComponent::ZoomUpdate(float ZoomValue)
{
	USpringArmComponent* OwnerSpringArm = GetOwner()->FindComponentByClass<USpringArmComponent>();
	if (OwnerSpringArm)
	{
		OwnerSpringArm->TargetArmLength = ZoomValue;
	}
}

void UMHGreatSwordComponent::BlendWeightUpdate(float BlendWeightValue)
{
	if (PostProcessMaterialDynamicInstance)
	{
		PostProcessMaterialDynamicInstance->SetScalarParameterValue(TEXT("Alpha"), BlendWeightValue);
	}
}

