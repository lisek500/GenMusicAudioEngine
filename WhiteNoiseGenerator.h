#pragma once
#include <random>

class WhiteNoiseGenerator
{
public:
	void prepare();

	float processSample();

	void setGain(float gain);

private:
	std::mt19937 rng_;

	std::uniform_real_distribution<float> dist_{ -1.0f, 1.0f };

	float gain_ = 1.0f;

};