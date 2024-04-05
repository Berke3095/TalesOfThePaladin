#include "Camera/ImpactCameraShake.h"

UImpactCameraShake::UImpactCameraShake() // Defaults for camera shake, defaults of sprint step camera shake
{
	// These are manipulated in MyCharacter class
	OscillationDuration = 0.2f;
	RotOscillation.Pitch.Amplitude = 20.0f;
	RotOscillation.Pitch.Frequency = 20.0f;
	LocOscillation.Y.Amplitude = 20.0f;
	LocOscillation.Y.Frequency = 20.0f;
}
