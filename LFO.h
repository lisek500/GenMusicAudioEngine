#pragma once
#include "WaveType.h"


class LFO
{
public:
	
	void prepare(double sampleRate);

	void setWaveType(WaveType waveType);

	void setFrequency(float frequency);

	float processSample();



private:

	double sampleRate_ = 48000.0;

	double phase_ = 0.0;

	double phaseIncrement_ = 0.0;

	float frequency_ = 0.1f;

	const float pi = 3.14159265358979323846f;

	WaveType waveType_ = WaveType::Sine;

};