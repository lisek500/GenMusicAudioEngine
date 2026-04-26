#pragma once
#include "AudioOutput.h"

class LowPassFilter
{
public:
	
	void prepare(double sampleRate);
	void reset();

	float processSample(float input);
	AudioOutput processSample(AudioOutput input);

	void setCutoffHz(float cutoffHz);

private:

	void updateCoefficient();

	double sampleRate_ = 48000.0;

	float cutoffHz_ = 1000.0f;
	float a_ = 0.0f;
	float z1_ = 0.0f;



};