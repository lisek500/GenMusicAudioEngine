#include "Synthesizer.h"
#include <cmath>

void Synthesizer::setADSR_amp(float attack, float decay, float sustain, float release)
{
	adsr_amp_.setAttack(attack);
	adsr_amp_.setDecay(decay);
	adsr_amp_.setSustain(sustain);
	adsr_amp_.setRelease(release);
}

void Synthesizer::setNumVoices(int voices)
{
	numVoices_ = voices;
}

void Synthesizer::setDetune(float cents)
{
	detune_ = cents;
}

void Synthesizer::prepare(double sampleRate, WaveType waveType, float width, int harmonics)
{
	//prepare components
	sampleRate_ = sampleRate;

	width_ = width;

	harmonics_ = harmonics;

	wave_ = waveType;


	//oscillator settings
	oscillators_.resize(numVoices_);

	for (int i = 0; i < numVoices_; i++)
	{
		oscillators_[i].prepare(sampleRate_);
		oscillators_[i].setWaveType(wave_);
		oscillators_[i].setPulseWidth(width_);
		oscillators_[i].setNumHarmonics(harmonics_);
	}



	adsr_amp_.prepare(sampleRate_);
	adsr_filter_.prepare(sampleRate_);
	lfo_.prepare(sampleRate_);



	panner_.setPan(0.5f);


	//ADSR amp settings
	adsr_amp_.setAttack(0.1f);
	adsr_amp_.setDecay(0.6f);
	adsr_amp_.setSustain(1.0f);
	adsr_amp_.setRelease(2.0f);

	//adsr filter settings
	adsr_filter_.setAttack(0.1f);
	adsr_filter_.setDecay(0.6f);
	adsr_filter_.setSustain(1.0f);
	adsr_filter_.setRelease(2.0f);


}

void Synthesizer::reset()
{
	adsr_amp_.reset();

	adsr_filter_.reset();

}

void Synthesizer::noteOn(Notes note, int octave)
{
	float freq = NotePitch::noteToFrequency(note, octave);
	noteOn(freq);
}

void Synthesizer::noteOn(float frequency)
{
	for (int i = 0; i < numVoices_; i++)
	{
		float spread = (float)i / (float)(numVoices_ - 1);
		float cents = (spread - 0.5f) * 2.0f * detune_;

		float freqMultiplier = pow(2.0f, cents / 1200.0f);

		oscillators_[i].setFrequency(frequency * freqMultiplier);
	}

	adsr_amp_.noteOn();
	adsr_filter_.noteOn();
}

void Synthesizer::noteOff()
{
	adsr_amp_.noteOff();
	adsr_filter_.noteOff();
}


AudioOutput Synthesizer::processSample()
{
	float amp = adsr_amp_.processSample();

	float mixedSignal = 0.0f;

	for (int i = 0; i < numVoices_; i++)
	{
		mixedSignal += oscillators_[i].processSample();
	}
	
	mixedSignal /= (float)numVoices_;
	
	
	
	
	float signal = mixedSignal *  amp * gain_;

	AudioOutput out = panner_.processSample(signal);

	return out;
}


//getters
ADSR& Synthesizer::getADSR_amp()
{
	return adsr_amp_;
}

ADSR& Synthesizer::getADSR_filter()
{
	return adsr_filter_;
}

Panner& Synthesizer::getPanner_()
{
	return panner_;
}

LFO& Synthesizer::getLFO()
{
	return lfo_;
}