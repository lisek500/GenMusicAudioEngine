#include "EnvFollower.h"
#include <cmath>

void EnvelopeFollower::prepare(double sampleRate)
{
	sampleRate_ = sampleRate;
	reset();
}

void EnvelopeFollower::reset()
{
	envelope_ = 0.0f;
}

float EnvelopeFollower::processSample(float input)
{
	float absX = std::abs(input);

	if (absX > envelope_)
	{
		envelope_ += attack_ * (absX - envelope_);
	}
	else
	{
		envelope_ += release_ * (absX - envelope_);
	}
	return envelope_;
}

float EnvelopeFollower::getEnvelope() const
{
	return envelope_;
}

void EnvelopeFollower::setAttack(float attack)
{
	attack_ = attack;
}

void EnvelopeFollower::setRelease(float release)
{
	release_ = release;
}