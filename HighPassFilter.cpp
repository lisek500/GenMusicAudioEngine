#include "HighPassFilter.h"
#include <cmath>
#include <algorithm>

void HighPassFilter::prepare(double sampleRate)
{
	sampleRate_ = sampleRate;
	updateCoefficient();
	reset();
}

void HighPassFilter::reset()
{
	y1_ = 0.0f;
	x1_ = 0.0f;
}

void HighPassFilter::updateCoefficient()
{
	const float pi = 3.14159265358979323846f;
	
	float rc = 1.0f / (2.0f * pi * cutoffHz_);
	float dt = 1.0f / sampleRate_;


	a_ = rc / (rc + dt);
}

void HighPassFilter::setCutoffHz(float cutoffHz)
{
	cutoffHz_ = std::clamp(cutoffHz, 20.0f, 20000.0f);
	updateCoefficient();
}

float HighPassFilter::processSample(float input)
{

	
	float y = a_ * (y1_ + input - x1_);
	x1_ = input;
	y1_ = y;
	return y;
}