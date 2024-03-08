// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/RealRecoilCameraShake.h"


URealRecoilCameraShake::URealRecoilCameraShake()
{
	OscillationDuration = 0.25f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = FMath::RandRange(0.5f, 1.0f);
	RotOscillation.Pitch.Frequency = FMath::RandRange(5.0f, 10.0f);
	
	RotOscillation.Yaw.Amplitude = FMath::RandRange(0.1f, 0.3f);
	RotOscillation.Yaw.Frequency = FMath::RandRange(5.0f, 10.0f);

	RotOscillation.Roll.Amplitude = FMath::RandRange(0.1f, 0.3f);
	RotOscillation.Roll.Frequency = FMath::RandRange(5.0f, 10.0f);
}
