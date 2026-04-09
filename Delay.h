#pragma once
#include <vector>

class Delay
{
	
public:	
	void prepare(double sampleRate, float maxDelayTimeMs);
	void reset();

	float processSample(float input);

	

	void setDelayTimeMs(float delayTimeMs);
	void setFeedback(float feedback);
	void setWetLevel(float wetLevel);

private:
	double sampleRate_ = 48000.0;

	std::vector<float> delayBuffer_;
	int writeIndex_ = 0;
	int delaySamples_ = 24000;

	float feedback_ = 0.01f;
	float wetLevel_ = 0.1f;

};