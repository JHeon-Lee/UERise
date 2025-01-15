// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/CameraShakeWeak.h"

UCameraShakeWeak::UCameraShakeWeak()	
{
	OscillationDuration = 0.3f;
	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.2;

	RotOscillation.Pitch.Amplitude = -2.0;
	RotOscillation.Pitch.Frequency = 50.0f;

	LocOscillation.X.Amplitude = 2.0f;
	LocOscillation.X.Frequency = 50.0f;
}
