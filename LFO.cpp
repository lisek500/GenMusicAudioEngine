#include "LFO.h"
#include <cmath>
#include <algorithm>

void LFO::prepare(double sampleRate)
{
	sampleRate_ = sampleRate;
}

void LFO::setWaveType(WaveType waveType)
{
	waveType_ = waveType;
}

void LFO::setFrequency(float frequency)
{
	frequency_ = frequency;

	phaseIncrement_ = (2 * pi * frequency_) / sampleRate_;
}

float LFO::processSample()
{
	float value = sin(phase_);

	phase_ += phaseIncrement_;

	if (phase_ >= 2 * pi)
	{
		phase_ -= 2 * pi;
	}
	switch (waveType_)
	{

	case WaveType::Sine:


		return value;


	case WaveType::Square:


		if (phase_ < pi)
		{
			return 1.0f;
		}
		else
		{
			return -1.0f;
		}

	}
	return 0.0f;
	
}