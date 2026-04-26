#pragma once
#include "Oscillator.h"
#include "AudioOutput.h"
#include "LFO.h"
#include "ADSR.h"
#include "Panner.h"
#include <vector>

class Synthesizer
{
public:
	void prepare(double sampleRate, WaveType waveType, float width, int harmonics );

	void setADSR_amp(float attack, float decay, float sustain, float release);

	void setNumVoices(int voices);

	void setDetune(float cents);

	void reset();

	void noteOn(Notes note, int octave);

	void noteOn(float frequency);

	void noteOff();

	AudioOutput processSample();

	ADSR& getADSR_amp();

	ADSR& getADSR_filter();

	Panner& getPanner_();

	LFO& getLFO();


private:
	std::vector<Oscillator> oscillators_;

	int numVoices_ = 1;

	float detune_ = 0.0f;

	ADSR adsr_amp_;

	ADSR adsr_filter_;

	Panner panner_;

	LFO lfo_;

	float gain_ = 0.5f;

	float width_ = 0.5f;

	int harmonics_ = 1;

	WaveType wave_ = WaveType::Sine;

	double sampleRate_ = 48000.0;
};