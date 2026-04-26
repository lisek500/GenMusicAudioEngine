#include "Delay.h"
#include <algorithm>


void Delay::prepare(double sampleRate, float maxDelayTimeMs)
{
	sampleRate_ = sampleRate;

	int maxSamples = static_cast<int>((maxDelayTimeMs / 1000.0f) * static_cast<float>(sampleRate_));
	if (maxSamples < 1)
	{
		maxSamples = 1;
	}

	delayBuffer_.assign(maxSamples + 1, 0.0f);
	writeIndex_ = 0;
	delaySamples_ = 1;

}

void Delay::reset()
{
	std::fill(delayBuffer_.begin(), delayBuffer_.end(), 0.0f);
	writeIndex_ = 0;
}

float Delay::processSample(float input)
{
	int readIndex = writeIndex_ - delaySamples_;
	if (readIndex < 0)
	{
		readIndex += static_cast<int>(delayBuffer_.size());
	}

	float delayedSample = delayBuffer_[readIndex];

	float toBuffer = input + delayedSample * feedback_;
	delayBuffer_[writeIndex_] = toBuffer;

	writeIndex_++;
	if (writeIndex_ >= static_cast<int>(delayBuffer_.size()))
	{
		writeIndex_ = 0;
	}

	float dry = input;
	float wet = delayedSample * wetLevel_;

	return dry + wet;
}

AudioOutput Delay::processSample(AudioOutput input)
{
	AudioOutput out;

	out.left = processSample(input.left);
	out.right = processSample(input.right);

	return out;
}

void Delay::setDelayTimeMs(float delayTimeMs)
{
	float samples = static_cast<float>((delayTimeMs / 1000.0) * sampleRate_);
	delaySamples_ = std::clamp(static_cast<int>(samples), 1, static_cast<int>(delayBuffer_.size()) - 1);
}

void Delay::setFeedback(float feedback)
{

	feedback_ = std::clamp(feedback, 0.0f, 1.25f);
}

void Delay::setWetLevel(float wetLevel)
{
	wetLevel_ = std::clamp(wetLevel, 0.0f, 1.0f);
}