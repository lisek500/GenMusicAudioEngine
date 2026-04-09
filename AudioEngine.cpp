#include "AudioEngine.h"
#include <cmath>
#include <algorithm>
#include <iostream>


void AudioEngine::prepare(double sampleRate, int blockSize)
{
	sampleRate_ = sampleRate;
	blockSize_ = blockSize;
	
	
	
	
	//envelope
	envelopeFollower_.setAttack(0.01f);
	envelopeFollower_.setRelease(0.001f);
	//oscillators
	oscillator_1.prepare(sampleRate_);
	oscillator_2.prepare(sampleRate_);
	oscillator_3.prepare(sampleRate_);

	//sequencer 
	BPM_ = 30.0f;

	//sequencer 1
	scale_1.setRoot(Notes::C, 3);
	scale_1.setScale(Scale::PENTATONIC_MAJOR);
	scaleSize_1 = scale_1.getScaleSize();
	sequencer_1.prepare(sampleRate_,1.0f, BPM_);
	sequencer_1.generateRandomSteps(16, scaleSize_1, NoteValue::WHOLE);


	//sequencer 2
	scale_2.setRoot(Notes::C, 4);
	scale_2.setScale(Scale::PENTATONIC_MAJOR);
	scaleSize_2 = scale_2.getScaleSize();
	sequencer_2.prepare(sampleRate_, 1.0f, BPM_);
	sequencer_2.generateRandomSteps(12, scaleSize_2, NoteValue::QUARTER);

	//sequencer 3
	scale_3.setRoot(Notes::C, 5);
	scale_3.setScale(Scale::PENTATONIC_MAJOR);
	scaleSize_3 = scale_3.getScaleSize();
	sequencer_3.prepare(sampleRate_, 1.0f, BPM_);
	sequencer_3.generateRandomSteps(16, scaleSize_3, NoteValue::SIXTEENTH);



	//LFO
	lfo_.prepare(sampleRate_);


	//lowpass filter
	lowPassFilter_.prepare(sampleRate_);
	lowPassFilter_2.prepare(sampleRate_);

	//envelope follower
	envelopeFollower_.prepare(sampleRate_);

	//delay
	delay_1.prepare(sampleRate_, 2000.0f);
	delay_2.prepare(sampleRate_, 2000.0f);
	delay_3.prepare(sampleRate_, 2000.0f);
	delay_4.prepare(sampleRate_, 2000.0f);

	reset();
}

void AudioEngine::reset()
{
	lowPassFilter_.reset();
	lowPassFilter_2.reset();
	highPassFilter_.reset();
	envelopeFollower_.reset();
	delay_1.reset();
	delay_2.reset();
	delay_3.reset();
	delay_4.reset();

}

AudioOutput AudioEngine::processSample(float input)
{
	float x = input * gain_;
	
	
	

	//float signal = lowpass + highpass;

	float env = envelopeFollower_.processSample(input);


	//noise
	float noise = noiseGenerator_.processSample();

	//Sequencer
	sequencer_1.update(env);
	if (sequencer_1.bConsumeLoopWrapped(true))
	{
		scale_1.setRoot(Notes::C, 3);
		scale_1.setScale(Scale::PENTATONIC_MAJOR);
		scaleSize_1 = scale_1.getScaleSize();
		sequencer_1.generateRandomSteps(16, scaleSize_1, NoteValue::WHOLE);
	}

	Step currentStep = sequencer_1.getCurrentStep();

	 float frequency = scale_1.getFrequencyFromScaleDegree(currentStep.scaleDegree);

	
	 //sequencer 2
	 sequencer_2.update(env);
	 if (sequencer_2.bConsumeLoopWrapped(true))
	 {
		 scale_2.setRoot(Notes::C, 4);
		 scale_2.setScale(Scale::PENTATONIC_MAJOR);
		 scaleSize_2 = scale_2.getScaleSize();
		 sequencer_2.generateRandomSteps(12, scaleSize_2, NoteValue::QUARTER);
	 }


	 Step currentStep_2 = sequencer_2.getCurrentStep();

	 float frequency_2 = scale_2.getFrequencyFromScaleDegree(currentStep_2.scaleDegree);


	 
	//sequencer 3
	 sequencer_3.update(env);
	 if (sequencer_3.bConsumeLoopWrapped(true))
	 {
		 scale_3.setRoot(Notes::C, 5);
		 scale_3.setScale(Scale::PENTATONIC_MAJOR);
		 scaleSize_3 = scale_1.getScaleSize();
		 sequencer_3.generateRandomSteps(16, scaleSize_3, NoteValue::SIXTEENTH);
	 }


	 Step currentStep_3 = sequencer_3.getCurrentStep();

	 float frequency_3 = scale_3.getFrequencyFromScaleDegree(currentStep_3.scaleDegree);


	 //std::cout << frequency;
	
	 


	//osciallators
	//float envValue = frequencyMin_ + env * (frequencyMax_ - frequencyMin_);
	
	//oscillator 1
	oscillator_1.setWaveType(WaveType::Sine);
	
	oscillator_1.setFrequency(frequency);

	float frequencyOsc1 = oscillator_1.processSample();

	//oscillator 2
	oscillator_2.setWaveType(WaveType::Sine);
	
	oscillator_2.setFrequency(frequency_2);

	float frequencyOsc2 = oscillator_2.processSample();

	//oscillator 3
	oscillator_3.setWaveType(WaveType::Sinc);

	
	oscillator_3.setFrequency(frequency_3);

	float frequencyOsc3 = oscillator_3.processSample();
	
	//LFO
	lfo_.setWaveType(WaveType::Sine);
	lfo_.setFrequency(0.6f);
	float lfoFreq = lfo_.processSample();


	

	//LFO Panning
	panner_ch1_.setPan(lfoFreq);
	panner_ch2_.setPan(1.0f - lfoFreq);

	/*panner_ch1_.setPan(0.5f);
	panner_ch2_.setPan(0.5f);*/

	//outputs
	float oscSignal_1 = /*env */ frequencyOsc1 * oscGain_1;
	float oscSignal_2 = /*env */ frequencyOsc2 * oscGain_2;
	float oscSignal_3 = env * frequencyOsc3 * oscGain_3;

	

	float noiseSignal = env * noise;

	//effects
	oscSignal_1 = lowPassFilter_.processSample(oscSignal_1);
	oscSignal_1 = delay_1.processSample(oscSignal_1);

	oscSignal_2 = lowPassFilter_2.processSample(oscSignal_2);
	oscSignal_2 = delay_2.processSample(oscSignal_2);

	lowPassFilter_3.setCutoffHz(500.0f);
	oscSignal_3 = lowPassFilter_3.processSample(oscSignal_3);
	oscSignal_3 = delay_3.processSample(oscSignal_3);
	
	
	noiseSignal = highPassFilter_.processSample(noiseSignal);
	noiseSignal = delay_4.processSample(noiseSignal);

	AudioOutput out1 = panner_ch1_.processSample(oscSignal_1 + oscSignal_2);
	AudioOutput out2 = panner_ch2_.processSample(oscSignal_3 + noiseSignal);

	AudioOutput sum;

	sum.left = out1.left + out2.left;
	sum.right = out1.right + out2.right;

	return sum;

}

float AudioEngine::getEnvelope() const
{
	return envelopeFollower_.getEnvelope();
}
void AudioEngine::setGain(float newGain)
{
	gain_ = newGain;
}

void AudioEngine::setOscGain(float gain1, float gain2, float gain3)
{
	oscGain_1 = gain1;
	oscGain_2 = gain2;
	oscGain_3 = gain3;
}

Panner& AudioEngine::getPanner_ch1()
{
	return panner_ch1_;
}

const Panner& AudioEngine::getPanner_ch1() const
{
	return panner_ch1_;
}

Panner& AudioEngine::getPanner_ch2()
{
	return panner_ch2_;
}

const Panner& AudioEngine::getPanner_ch2() const
{
	return panner_ch2_;
}

Sequencer& AudioEngine::getSequencer()
{
	return sequencer_1;
}

const Sequencer& AudioEngine::getSequencer() const
{
	return sequencer_1;
}

Sequencer& AudioEngine::getSequencer_2()
{
	return sequencer_2;
}

const Sequencer& AudioEngine::getSequencer_2() const
{
	return sequencer_2;
}

Sequencer& AudioEngine::getSequencer_3()
{
	return sequencer_3;
}

const Sequencer& AudioEngine::getSequencer_3() const
{
	return sequencer_3;
}

ScaleClass& AudioEngine::getScale()
{
	return scale_1;
}

const ScaleClass& AudioEngine::getScale() const
{
	return scale_1;
}

ScaleClass& AudioEngine::getScale_2()
{
	return scale_2;
}

const ScaleClass& AudioEngine::getScale_2() const
{
	return scale_2;
}

ScaleClass& AudioEngine::getScale_3()
{
	return scale_3;
}

const ScaleClass& AudioEngine::getScale_3() const
{
	return scale_3;
}

WhiteNoiseGenerator& AudioEngine::getWhiteNoiseGenerator()
{
	return noiseGenerator_;
}

const WhiteNoiseGenerator& AudioEngine::getWhiteNoiseGenerator() const
{
	return noiseGenerator_;
}

Oscillator& AudioEngine::getOscillator_1()
{
	return oscillator_1;
}

const Oscillator& AudioEngine::getOscillator_1() const
{
	return oscillator_1;
}

Oscillator& AudioEngine::getOscillator_2()
{
	return oscillator_2;
}

const Oscillator& AudioEngine::getOscillator_2() const
{
	return oscillator_2;
}

Oscillator& AudioEngine::getOscillator_3()
{
	return oscillator_3;
}

const Oscillator& AudioEngine::getOscillator_3() const
{
	return oscillator_3;
}

LFO& AudioEngine::getLFO()
{
	return lfo_;
}

const LFO& AudioEngine::getLFO() const
{
	return lfo_;
}

LowPassFilter& AudioEngine::getLowPassFilter()
{
	return lowPassFilter_;
}

const LowPassFilter& AudioEngine::getLowPassFilter() const
{
	return lowPassFilter_;
}

LowPassFilter& AudioEngine::getLowPassFilter_2()
{
	return lowPassFilter_2;
}

const LowPassFilter& AudioEngine::getLowPassFilter_2() const
{
	return lowPassFilter_2;
}

LowPassFilter& AudioEngine::getLowPassFilter_3()
{
	return lowPassFilter_3;
}

const LowPassFilter& AudioEngine::getLowPassFilter_3() const
{
	return lowPassFilter_3;
}

HighPassFilter& AudioEngine::getHighPassFilter()
{
	return highPassFilter_;
}

const HighPassFilter& AudioEngine::getHighPassFilter() const
{
	return highPassFilter_;
}

EnvelopeFollower& AudioEngine::getEnvelopeFollower()
{
	return envelopeFollower_;
}

const EnvelopeFollower& AudioEngine::getEnvelopeFollower() const
{
	return envelopeFollower_;
}

Delay& AudioEngine::getDelay_1()
{
	return delay_1;
}

const Delay& AudioEngine::getDelay_1() const
{
	return delay_1;
}

Delay& AudioEngine::getDelay_2()
{
	return delay_2;
}

const Delay& AudioEngine::getDelay_2() const
{
	return delay_2;
}

Delay& AudioEngine::getDelay_3()
{
	return delay_3;
}

const Delay& AudioEngine::getDelay_3() const
{
	return delay_3;
}

Delay& AudioEngine::getDelay_4()
{
	return delay_4;
}

const Delay& AudioEngine::getDelay_4() const
{
	return delay_4;
}
