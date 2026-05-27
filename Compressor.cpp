#include "Compressor.h"
#include <cmath>
#include <algorithm>
#include <stdlib.h>
#include <iostream>


void Compressor::prepare(double sampleRate, bool limiter, int lookahead)
{
	sampleRate_ = sampleRate;
	bLimiter_ = limiter;
	writeIndex_ = 0;
	lookaheadSamples_ = lookahead;

	reset();
}

void Compressor::reset()
{
	envelope_ = 0.0;
	writeIndex_ = 0;
}

float Compressor::getEnvelope() const
{
	return envelope_;
}

void Compressor::setThreshold(float threshold)
{
	threshold_ = threshold;
}

void Compressor::setRatio(float ratio)
{
	ratio_ = ratio;
}

void Compressor::setAttack(float attack)
{
	attack_ = attack;
}

void Compressor::setRelease(float release)
{
	release_ = release;
}

float Compressor::processSample(float input)
{	
	if (bLimiter_)
	{
		readIndex_ = writeIndex_ - lookaheadSamples_;
		if (readIndex_ < 0)
		{
			readIndex_ += static_cast<int>(buffer_.size());
		}

		 delayedSample_ = buffer_[readIndex_];


		buffer_[writeIndex_] = input;

		writeIndex_++;
		if (writeIndex_ >= static_cast<int>(buffer_.size()))
		{
			writeIndex_ = 0;
		}

	}
	
	float inputMax = std::max(abs(input), 1e-6f);

	float inputdB = 20 * log10(inputMax);

	//std::cout << inputdB;

	if (inputdB > threshold_)
	{
		if (bLimiter_)
		{
			 gainReductiondB_ = inputdB - threshold_;
		}
		else
		{
			gainReductiondB_ = (inputdB - threshold_) * (1.0f - 1.0f / ratio_);
		}
		
		
		if (gainReductiondB_ > envelope_)
		{
			envelope_ += attack_ * (gainReductiondB_ - envelope_);
		}
		else
		{
			envelope_ += release_ * (gainReductiondB_ - envelope_);
		}

		float gain = pow(10, - envelope_ / 20);

		return (bLimiter_ ? delayedSample_ : input) * gain;
		
	}
	else
	{
		envelope_ += release_ * (0.0f - envelope_);
		float gain = pow(10, -envelope_ / 20);
		return (bLimiter_ ? delayedSample_ : input) * gain;

		
	}
}

AudioOutput Compressor::processSample(AudioOutput input)
{
	AudioOutput out;

	out.left = processSample(input.left);
	out.right = processSample(input.right);

	return out;
}
