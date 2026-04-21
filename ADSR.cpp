#include "ADSR.h"
#include <cmath>

void ADSR::prepare(double sampleRate)
{
	sampleRate_ = sampleRate;
}

void ADSR::reset()
{
	envelope_ = 0.0f;
	state_ = ADSR_State::Idle;
}

void ADSR::setAttack(float attack)
{
	attack_ = attack;
}

void ADSR::setDecay(float decay)
{
	decay_ = decay;
}

void ADSR::setSustain(float sustain)
{
	sustain_ = sustain;
}

void ADSR::setRelease(float release)
{
	release_ =  release;
}

void ADSR::noteOn()
{
	state_ = ADSR_State::Attack;
	envelope_ = 0.0f;
}

void ADSR::noteOff()
{
	state_ = ADSR_State::Release;
}

float ADSR::processSample()
{
	switch (state_)
	{
	case ADSR_State::Attack:
	{
		float attackIncrement = 1.0f / (attack_ * sampleRate_);
		envelope_ += attackIncrement;

		if (envelope_ >= 1.0f)
		{
			envelope_ = 1.0f;
			state_ = ADSR_State::Decay;
		}
		break;
	}
	case ADSR_State::Decay:
	{
		float decayDecrement = (1.0f - sustain_) / (decay_ * sampleRate_);
		envelope_ -= decayDecrement;

		if (envelope_ <= sustain_)
		{
			envelope_ = sustain_;
			state_ = ADSR_State::Sustain;
		}
		break;

	}
	case ADSR_State::Sustain:
	{
		envelope_ = sustain_;
		break;
	}
	case ADSR_State::Release:
	{
		float releaseDecrement = envelope_ / (release_ * sampleRate_);
		envelope_ -= releaseDecrement;

		if (envelope_ <= 0.0f)
		{
			envelope_ = 0.0f;
			state_ = ADSR_State::Idle;
		}
		break;
	}
	case ADSR_State::Idle:
	{
		envelope_ = 0.0f;
		break;
	}


	}
	return envelope_;
} 