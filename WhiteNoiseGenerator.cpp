#include "WhiteNoiseGenerator.h"

void WhiteNoiseGenerator::prepare()
{
	rng_.seed(-1);
}

void WhiteNoiseGenerator::setGain(float gain)
{
	gain_ = gain;
}

float WhiteNoiseGenerator::processSample()
{
	return dist_(rng_) * gain_;
}
