#pragma once

enum class ADSR_State
{
	Idle,
	Attack,
	Decay,
	Sustain,
	Release,


};

class ADSR
{
public:

	void prepare(double sampleRate);

	void reset();

	void setAttack(float attack);

	void setDecay(float decay);

	void setSustain(float sustain);

	void setRelease(float release);

	void noteOn();

	void noteOff();

	float processSample();

private:

	double sampleRate_ = 48000.0;

	float attack_ = 0.1f;

	float decay_ = 0.1f;

	float sustain_ = 1.0f;

	float release_ = 0.1f;

	ADSR_State state_ = ADSR_State::Idle;

	float envelope_ = 0.0f;

};