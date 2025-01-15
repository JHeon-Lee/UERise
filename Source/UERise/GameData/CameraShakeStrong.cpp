// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/CameraShakeStrong.h"

UCameraShakeStrong::UCameraShakeStrong()
{
	OscillationDuration = 0.5f;
	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.2;

	RotOscillation.Pitch.Amplitude = -5.0;
	RotOscillation.Pitch.Frequency = 50.0f;
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;

	LocOscillation.X.Amplitude = 5.0f;
	LocOscillation.X.Frequency = 50.0f;
}
