#pragma once
#include <array>
#include "AudioOutput.h"

class Compressor
{
public:

	void prepare(double sampleRate, bool limiter, int lookahead);

	void reset();

	float processSample(float input);
	AudioOutput processSample(AudioOutput input);


	float getEnvelope() const;

	void setThreshold(float threshold);

	void setRatio(float ratio);

	void setAttack(float attack);
	void setRelease(float release);

private:
	double sampleRate_ = 48000.0;

	//circular buffer
	std::array<float, 512> buffer_;

	int writeIndex_;
	int readIndex_;
	int lookaheadSamples_;
	float delayedSample_;

	bool bLimiter_ = false;

	float gainReductiondB_ = 0.0f;

	float ratio_ = 4.0f;

	float threshold_ = 0.0f;

	float envelope_ = 0.0f;
	float attack_ = 0.01f;
	float release_ = 0.001f;


};