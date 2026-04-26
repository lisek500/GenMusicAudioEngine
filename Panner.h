#pragma once
#include "AudioOutput.h"

class Panner
{
public:

	void setPan(float pan);

	AudioOutput processSample(float input);
	AudioOutput processSample(AudioOutput input);

private:

	float pan_ = 0.0f;

	static constexpr float pi = 3.14159265358979323846f;

};