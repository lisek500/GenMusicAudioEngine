#include "Oscillator.h"
#include <cmath>
#include <algorithm>
#include <iostream>

void Oscillator::prepare(double sampleRate)
{
	sampleRate_ = sampleRate;

	phaseIncrement_ = (2 * pi * frequency_) / sampleRate_;

	waveTable_.resize(static_cast<int>(4096));
	for (size_t i = 0; i < waveTable_.size(); i++)
	{
		waveTable_[i] = std::sin(2 * pi  * i / waveTable_.size());
	}
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
	int index = static_cast<int>(phase_ / (2 * pi) * waveTable_.size());
	index = std::min(index, static_cast<int>(waveTable_.size()) - 1);
	
	int index1 = index;
	int index0 = (index1 - 1 + waveTable_.size()) % waveTable_.size();
	int index2 = (index1 + 1) % waveTable_.size();
	int index3 = (index2 + 1) % waveTable_.size();

	float phaseDiff = (phase_ / (2 * pi)) * waveTable_.size() - index1;

	//float interp1 = 1.0f - phaseDiff;

	//float interp2 = phaseDiff;
	
	
	//value = 0.5 * ((2*p1) + (-p0 + p2)*t + (2*p0 - 5*p1 + 4*p2 - p3)*t² + (-p0 + 3*p1 - 3*p2 + p3)*t³)
	float value = 0.5 * ((2 * waveTable_[index1]) +  (-waveTable_[index0] + waveTable_[index2]) 
		* phaseDiff + (2 * waveTable_[index0] - 5 * waveTable_[index1] + 4 * waveTable_[index2] - waveTable_[index3]) * phaseDiff * phaseDiff 
		+ (- waveTable_[index0] + 3 * waveTable_[index1] - 3 * waveTable_[index2] + waveTable_[index3]) * phaseDiff * phaseDiff * phaseDiff);

	//std::cout << phaseDiff;
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

