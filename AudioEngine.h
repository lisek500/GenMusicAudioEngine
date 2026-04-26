#pragma once
#include "Delay.h"
#include "EnvFollower.h"
#include "LowPassFilter.h"
#include "HighPassFilter.h"
#include "WhiteNoiseGenerator.h"
#include "Oscillator.h"
#include "Synthesizer.h"
#include "Panner.h"
#include "LFO.h"
#include "Sequencer.h"
#include "Scale.h"
#include "Compressor.h"
#include "MarkovChain.h"





class AudioEngine
{
public:
	void prepare(double sampleRate, int blockSize, float BPM);
	void reset();

	AudioOutput processSample(float input);


	void setGain(float newGain);

	void setOscGain(float gain1, float gain2, float gain3);
	
	float getEnvelope() const;

	Compressor& getCompressorLeft();
	const Compressor& getCompressorLeft() const;

	Compressor& getCompressorRight();
	const Compressor& getCompressorRight() const;

	Panner& getPanner_ch1();
	const Panner& getPanner_ch1() const;

	Panner& getPanner_ch2();
	const Panner& getPanner_ch2() const;

	Panner& getPanner_mono();
	const Panner& getPanner_mono() const;

	MarkovChain& getMarkov_1();

	Sequencer& getSequencer();
	const Sequencer& getSequencer() const;

	Sequencer& getSequencer_2();
	const Sequencer& getSequencer_2() const;

	Sequencer& getSequencer_3();
	const Sequencer& getSequencer_3() const;

	Sequencer& getSequencer_4();

	ScaleClass& getScale();
	const ScaleClass& getScale() const;

	ScaleClass& getScale_2();
	const ScaleClass& getScale_2() const;

	ScaleClass& getScale_3();
	const ScaleClass& getScale_3() const;

	WhiteNoiseGenerator& getWhiteNoiseGenerator();
	const WhiteNoiseGenerator& getWhiteNoiseGenerator() const;

	Synthesizer& getSynth_1();

	Synthesizer& getSynth_2();

	Synthesizer& getSynth_3();

	Synthesizer& getSynth_4();

	LFO& getLFO();
	const LFO& getLFO() const;

	LowPassFilter& getLowPassFilter();
	const LowPassFilter& getLowPassFilter() const;

	LowPassFilter& getLowPassFilter_2();
	const LowPassFilter& getLowPassFilter_2() const;

	LowPassFilter& getLowPassFilter_3();
	const LowPassFilter& getLowPassFilter_3() const;

	HighPassFilter& getHighPassFilter();
	const HighPassFilter& getHighPassFilter() const;
	
	EnvelopeFollower& getEnvelopeFollower();
	const EnvelopeFollower& getEnvelopeFollower() const;
	
	Delay& getDelay_1();
	const Delay& getDelay_1() const;

	Delay& getDelay_2();
	const Delay& getDelay_2() const;

	Delay& getDelay_3();
	const Delay& getDelay_3() const;

	Delay& getDelay_4();
	const Delay& getDelay_4() const;

private:
	int getEnergyState(float env);

	double sampleRate_ = 48000.0;
	int blockSize_ = 64;

	float gain_ = 1.0f;

	float oscGain_1 = 0.0f;

	float oscGain_2 = 0.0f;

	float oscGain_3 = 0.0f;

	float BPM_ = 60.0f;

	int scaleSize_1 = 0;

	int scaleSize_2 = 0;

	int scaleSize_3 = 0;

	int lastEnergyStateForRhythm_ = -1;

	int rhythmCooldown_ = 0;

	int lastStepIndex_1 = -1;

	int lastStepIndex_4 = -1;

	int lastScaleDegree_1 = -1;

	int lastScaleDegree_2 = -1;

	int lastScaleDegree_3 = -1;

	//float frequencyMin_ = 110.0f;

	//float frequencyMax_ = 110.0f;
	
	Compressor compressor_left_;

	Compressor compressor_right_;

	Panner panner_ch1_;

	Panner panner_ch2_;

	Panner panner_mono;

	MarkovChain markov_1;

	Sequencer sequencer_1;

	Sequencer sequencer_2;

	Sequencer sequencer_3;

	Sequencer sequencer_4;

	ScaleClass scale_1;

	ScaleClass scale_2;

	ScaleClass scale_3;

	WhiteNoiseGenerator noiseGenerator_;

	Synthesizer synth_1;

	Synthesizer synth_2;

	Synthesizer synth_3;

	Synthesizer synth_4;

	LFO lfo_;

	LowPassFilter  lowPassFilter_;

	LowPassFilter lowPassFilter_2;

	LowPassFilter lowPassFilter_3;

	HighPassFilter highPassFilter_;

	EnvelopeFollower envelopeFollower_;

	Delay delay_1;

	Delay delay_2;

	Delay delay_3;

	Delay delay_4;
};