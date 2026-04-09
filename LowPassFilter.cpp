#include "LowPassFilter.h"
#include <cmath>
#include <algorithm>

void LowPassFilter::prepare(double sampleRate)
{
	sampleRate_ = sampleRate;
	
	reset();
}
void LowPassFilter::updateCoefficient()
{
	const float pi = 3.14159265358979323846f;

	a_ = 1 - exp(-2 * pi * cutoffHz_ / sampleRate_);
}

void LowPassFilter::reset()
{
	z1_ = 0.0f;
}

void LowPassFilter::setCutoffHz(float cutoffHz)
{
	cutoffHz_ = std::clamp(cutoffHz, 20.0f, 20000.0f);
	updateCoefficient();
}



float LowPassFilter::processSample(float input)
{

	z1_ = z1_ + a_ * (input - z1_);
	
	return z1_;
}

