#pragma once
#include "WaveType.h"
#include "NotePitch.h"
#include <vector>


class Oscillator
{

public:

	void prepare(double sampleRate);

	void setWaveType(WaveType waveType);

	void setFrequency(Notes note, int octave);

	void setFrequency(float frequency);

	float sinc(float x) const;

	float processSample();

private:

	std::vector<float> waveTable_;

	double phase_ = 0.0;

	float frequency_ = 440.0f;

	double sampleRate_ = 48000.0;

	double phaseIncrement_ = 0.0;

	const float pi = 3.14159265358979323846f;

	WaveType waveType_ = WaveType::Sine;

	const float epsilon_ = 0.0001f;
	
};