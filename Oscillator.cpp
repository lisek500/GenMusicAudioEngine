#include "Oscillator.h"
#include <cmath>
#include <algorithm>

void Oscillator::prepare(double sampleRate)
{
	sampleRate_ = sampleRate;

	phaseIncrement_ = (2 * pi * frequency_) / sampleRate_;

	
}

//pitch


void Oscillator::setWaveType(WaveType waveType)
{
	waveType_ = waveType;
}




void Oscillator::setFrequency(float frequency)
{

	frequency_ = frequency;

	phaseIncrement_ = (2 * pi * frequency_) / sampleRate_;
}

void Oscillator::setFrequency(Notes note, int octave)
{
	setFrequency(NotePitch::noteToFrequency(note, octave));
}

float Oscillator::sinc(float x) const
{
	if (std::abs(x) < epsilon_)
	{
		return 1.0f;
	}
	else
	{
		return std::sin(x) / x;
	}

}

float Oscillator::processSample()
{
	float value = std::sin(phase_);

	float x = phase_ - pi;

	phase_ += phaseIncrement_;

	

	
	
	if (phase_ >= 2 * pi)
	{
		phase_ -= 2 * pi;
	}


	switch (waveType_) 
	{
	
	case WaveType::Sine:
		
		
		return value;
		
		
	case WaveType::Square:
		
		
		if (phase_ < pi)
		{
			return 1.0f;
		}
		else
		{
			return -1.0f;
		}

	case WaveType::Sinc: return sinc(x);

	}
	return 0.0f;

}

