// Fill out your copyright notice in the Description page of Project Settings.


#include "MHGameInstance.h"

UMHGameInstance::UMHGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UMHDataContainer> DataContainer(TEXT("/Script/UERise.MHDataContainer'/Game/Characters/Utusi/Data/MHDataBox.MHDataBox'"));

	if (DataContainer.Object)
	{
		MHDataContainerAsset = DataContainer.Object;
	}

}
