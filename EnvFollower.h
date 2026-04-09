#pragma once

class EnvelopeFollower
{
public:
	void prepare(double sampleRate);
	void reset();

	float processSample(float input);
	float getEnvelope() const;

	void setAttack(float attack);
	void setRelease(float release);


private:
	double sampleRate_ = 48000.0;

	float envelope_ = 0.0f;
	float attack_ = 0.01f;
	float release_ = 0.001f;
};