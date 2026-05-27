#include "AudioEngine.h"
#include <portaudio.h>
#include <iostream>


void AudioEngine::prepare(double sampleRate, int blockSize, float BPM)
{
	sampleRate_ = sampleRate;
	blockSize_ = blockSize;
	
	
	//compressor and limiter
	compressor_left_.prepare(sampleRate_, true, 5);
	compressor_right_.prepare(sampleRate_, true, 5);

	compressor_left_.setAttack(0.01f);
	compressor_right_.setAttack(0.01f);

	compressor_left_.setRelease(0.01f);
	compressor_right_.setRelease(0.01f);

	compressor_left_.setRatio(4.0f);
	compressor_right_.setRatio(4.0f);

	compressor_left_.setThreshold(-1.5f);
	compressor_right_.setThreshold(-1.5f);

	
	
	//envelope
	envelopeFollower_.setAttack(0.01f);
	envelopeFollower_.setRelease(0.001f);


	//synthesizers
	synth_1.setNumVoices(5);
	synth_1.setDetune(1.0f);
	synth_1.prepare(sampleRate_, WaveType::Sine, 0.5f, 8);
	synth_1.setADSR_amp(0.9f, 0.001f, 1.0f, 0.01f);
	
	synth_2.setNumVoices(5);
	synth_2.setDetune(10.0f);
	synth_2.prepare(sampleRate_, WaveType::Pulse, 0.1f, 32);
	synth_2.setADSR_amp(0.001f, 0.001f, 1.0f, 0.5f);

	synth_3.setNumVoices(5);
	synth_3.setDetune(10.0f);
	synth_3.prepare(sampleRate_, WaveType::Pulse, 0.1f, 32);
	synth_3.setADSR_amp(0.001f, 0.001f, 2.0f, 0.5f);

	synth_4.setNumVoices(12);
	synth_4.setDetune(20.0f);
	synth_4.prepare(sampleRate_, WaveType::Noise, 0.1f, 32);
	synth_4.setADSR_amp(0.001f, 0.05f, 0.1f, 0.01f);

	//Markov
	markov_1.prepare(4, 5, 5);
	//set state 0 note porbability
	markov_1.setNoteProbability(0, 0, 0.4f);
	markov_1.setNoteProbability(0, 1, 0.4f);
	markov_1.setNoteProbability(0, 2, 0.1f);
	markov_1.setNoteProbability(0, 3, 0.05f);
	markov_1.setNoteProbability(0, 4, 0.05f);

	//set state 1 note probability
	markov_1.setNoteProbability(1, 0, 0.1f);
	markov_1.setNoteProbability(1, 1, 0.05f);
	markov_1.setNoteProbability(1, 2, 0.4f);
	markov_1.setNoteProbability(1, 3, 0.1f);
	markov_1.setNoteProbability(1, 4, 0.05f);

	//set state 2 note probabilty
	markov_1.setNoteProbability(2, 0, 0.1f);
	markov_1.setNoteProbability(2, 1, 0.4f);
	markov_1.setNoteProbability(2, 2, 0.05f);
	markov_1.setNoteProbability(2, 3, 0.05f);
	markov_1.setNoteProbability(2, 4, 0.4f);

	//set state 3 note probabilty
	markov_1.setNoteProbability(3, 0, 0.4f);
	markov_1.setNoteProbability(3, 1, 0.4f);
	markov_1.setNoteProbability(3, 2, 0.1f);
	markov_1.setNoteProbability(3, 3, 0.05f);
	markov_1.setNoteProbability(3, 4, 0.05f);

	//markov 1 rhythm probabilities
	//rhythm state 0 probability
	markov_1.setRhythmProbability(0, 0, 1.0f);
	markov_1.setRhythmProbability(0, 1, 0.0f);
	markov_1.setRhythmProbability(0, 2, 0.0f);
	markov_1.setRhythmProbability(0, 3, 0.00f);
	markov_1.setRhythmProbability(0, 4, 0.00f);

	//rhythm state 1 probability
	markov_1.setRhythmProbability(1, 0, 0.0f);
	markov_1.setRhythmProbability(1, 1, 1.0f);
	markov_1.setRhythmProbability(1, 2, 0.0f);
	markov_1.setRhythmProbability(1, 3, 0.0f);
	markov_1.setRhythmProbability(1, 4, 0.0f);

	//rhythm state 2 probability
	markov_1.setRhythmProbability(2, 0, 0.0f);
	markov_1.setRhythmProbability(2, 1, 0.0f);
	markov_1.setRhythmProbability(2, 2, 1.0f);
	markov_1.setRhythmProbability(2, 3, 0.00f);
	markov_1.setRhythmProbability(2, 4, 0.00f);

	//rhythm state 3 probability 
	markov_1.setRhythmProbability(3, 0, 0.0f);
	markov_1.setRhythmProbability(3, 1, 0.0f);
	markov_1.setRhythmProbability(3, 2, 0.0f);
	markov_1.setRhythmProbability(3, 3, 0.5f);
	markov_1.setRhythmProbability(3, 4, 0.5f);

	//sequencer
	BPM_ = BPM;

	//sequencer 1
	scale_1.setRoot(Notes::A, 2);
	scale_1.setScale(Scale::PENTATONIC_MINOR);
	scaleSize_1 = scale_1.getScaleSize();
	sequencer_1.prepare(sampleRate_,1.0f, BPM_);
	sequencer_1.generateRandomSteps(16, scaleSize_1, NoteValue::WHOLE);


	//sequencer 2
	scale_2.setRoot(Notes::E, 4);
	scale_2.setScale(Scale::PENTATONIC_MINOR);
	scaleSize_2 = scale_2.getScaleSize();
	sequencer_2.prepare(sampleRate_, 1.0f, BPM_);
	sequencer_2.generateRandomSteps(12, scaleSize_2, NoteValue::QUARTER);

	//sequencer 3
	scale_3.setRoot(Notes::E, 5);
	scale_3.setScale(Scale::PENTATONIC_MINOR);
	scaleSize_3 = scale_3.getScaleSize();
	sequencer_3.prepare(sampleRate_, 1.0f, BPM_);
	sequencer_3.generateRandomSteps(16, scaleSize_3, NoteValue::SIXTEENTH);

	sequencer_4.prepare(sampleRate_, 1.0f, BPM_);
	sequencer_4.generateRandomSteps(16, scaleSize_3, NoteValue::SIXTEENTH);




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
	compressor_left_.reset();
	compressor_right_.reset();
	synth_1.reset();

}

AudioOutput AudioEngine::processSample(float input)
{

	float x = input * gain_;
	
	//float signal = lowpass + highpass;

	float env = envelopeFollower_.processSample(input);
	int energyState = getEnergyState(env);
	markov_1.setState(energyState);
	
	/*static int debugCounter = 0;
	debugCounter++;
	if (debugCounter % 48000 == 0)
	{
		std::cout << "env:" << env << "energy state:" << energyState << std::endl;
	}*/


	//noise
	float noise = noiseGenerator_.processSample();

	//Sequencer
	sequencer_1.update(env);
	if (sequencer_1.bConsumeLoopWrapped(true))
	{
		scale_1.setRoot(Notes::A, 2);
		scale_1.setScale(Scale::PENTATONIC_MINOR);
		scaleSize_1 = scale_1.getScaleSize();
		//sequencer_1.generateRandomSteps(16, scaleSize_1, NoteValue::WHOLE);
	}

	//Step currentStep = sequencer_1.getCurrentStep();
	
	int currentStepIndex = sequencer_1.getCurrentStepIndex();

	/*static int lastPrinted = -1;
	if (currentStepIndex != lastPrinted)
	{
		std::cout << "Step changed: " << lastPrinted << " -> " << currentStepIndex << std::endl;
		lastPrinted = currentStepIndex;
	}*/

	if (currentStepIndex != lastStepIndex_1)
	{

		int noteIndex_1 = markov_1.getNextNote();
		//std::cout << "State: " << energyState << " Note:" << noteIndex_1 << std::endl;
		std::cout << "Synth_1 - state" << energyState
			<< " NoteIndex: " << noteIndex_1
			<< " StepIndex: " << currentStepIndex << std::endl;
		
		float frequency = scale_1.getFrequencyFromScaleDegree(noteIndex_1);
		synth_1.noteOn(frequency);
		if (rhythmCooldown_ > 0)
		{
			rhythmCooldown_--;
		}

		lastStepIndex_1 = currentStepIndex;
	}

	if (energyState != lastEnergyStateForRhythm_ && rhythmCooldown_ <= 0)
	{
		NoteValue rhythmIndex_1 = markov_1.getNextDuration();
		//std::cout << "Rhythm Index: " << static_cast<int>(rhythmIndex_1) << std::endl;
		sequencer_1.setAllStepsDuration(rhythmIndex_1);

		lastEnergyStateForRhythm_ = energyState;
		rhythmCooldown_ = 16;
	}
	
	 //sequencer 2
	 sequencer_2.update(env);
	 if (sequencer_2.bConsumeLoopWrapped(true))
	 {
		 scale_2.setRoot(Notes::E, 4);
		 scale_2.setScale(Scale::PENTATONIC_MINOR);
		 scaleSize_2 = scale_2.getScaleSize();
		 sequencer_2.generateRandomSteps(12, scaleSize_2, NoteValue::QUARTER);
	 }


	 Step currentStep_2 = sequencer_2.getCurrentStep();
	 float frequency_2 = scale_2.getFrequencyFromScaleDegree(currentStep_2.scaleDegree);

	 if (currentStep_2.scaleDegree != lastScaleDegree_2)
	 {
		 synth_2.noteOn(frequency_2);
		 lastScaleDegree_2 = currentStep_2.scaleDegree;
	 }
	 
	//sequencer 3
	 sequencer_3.update(env);
	 if (sequencer_3.bConsumeLoopWrapped(true))
	 {
		 scale_3.setRoot(Notes::E, 5);
		 scale_3.setScale(Scale::PENTATONIC_MINOR);
		 scaleSize_3 = scale_1.getScaleSize();
		 sequencer_3.generateRandomSteps(16, scaleSize_3, NoteValue::SIXTEENTH);
	 }


	 Step currentStep_3 = sequencer_3.getCurrentStep();
     float frequency_3 = scale_3.getFrequencyFromScaleDegree(currentStep_3.scaleDegree);

	 if (currentStep_3.scaleDegree != lastScaleDegree_3)
	 {
		 synth_3.noteOn(frequency_3);
		 lastScaleDegree_2 = currentStep_3.scaleDegree;
	 }

	 //sequencer 4
	 
	 int currentStepIndex_4 = sequencer_4.getCurrentStepIndex();

	 if (currentStepIndex_4 != lastStepIndex_4)
	 {
		 float randomFreq = 200.f + (rand() % 800);
		 synth_4.noteOn(randomFreq);

		 lastStepIndex_4 = currentStepIndex_4;
	 }

	 //std::cout << frequency;
	
	 


	//osciallators
	//float envValue = frequencyMin_ + env * (frequencyMax_ - frequencyMin_);
	
	//Synth 1
	AudioOutput out_1 = synth_1.processSample();

	//Synth 2
	AudioOutput out_2 = synth_2.processSample();

	//Synth 3
	AudioOutput out_3 = synth_3.processSample();

	//Synth 4
	AudioOutput out_4 = synth_4.processSample();

	

	
	//LFO
	lfo_.setWaveType(WaveType::Sine);
	lfo_.setFrequency(0.09f);
	float lfoFreq = lfo_.processSample();


	

	//LFO Panning
	float panValue = (lfoFreq + 1.0f) / 2.0f;
	panner_ch1_.setPan(panValue);
	panner_ch2_.setPan(1.0f - panValue);
	panner_mono.setPan(0.0f);

	/*panner_ch1_.setPan(0.5f);
	panner_ch2_.setPan(0.5f);*/

	//output calculations
	float oscSignal_1 = /*env */ out_1.left * oscGain_1;
	AudioOutput oscSignal_2 = /*env */ out_2 * oscGain_2;
	AudioOutput oscSignal_3 = out_3 * oscGain_3;
	AudioOutput oscSignal_4 = out_4 * oscGain_3;

	//envelope calculations
	oscSignal_3.left *= env;
	oscSignal_3.right *= env;

	oscSignal_4.left *= env;
	oscSignal_4.right *= env;


	

	//float noiseSignal = env * noise;

	//effects
	out_1 = lowPassFilter_.processSample(out_1);
	out_1 = delay_1.processSample(out_1);

	oscSignal_2 = lowPassFilter_2.processSample(oscSignal_2);
	oscSignal_2 = delay_2.processSample(oscSignal_2);

	lowPassFilter_3.setCutoffHz(500.0f);
	oscSignal_3 = lowPassFilter_3.processSample(oscSignal_3);
	oscSignal_3 = delay_3.processSample(oscSignal_3);
	
	
	oscSignal_4 = highPassFilter_.processSample(oscSignal_4);
	oscSignal_4 = delay_4.processSample(oscSignal_4);

	//AudioOutput noiseStereo;
	//noiseStereo.left = noiseSignal;
	//noiseStereo.right = noiseSignal;

	AudioOutput out1 = panner_ch1_.processSample(oscSignal_2);
	AudioOutput out2 = panner_ch2_.processSample(oscSignal_3 + oscSignal_4);
	AudioOutput mono = out_1;

	AudioOutput sum;


	sum.left = out1.left + out2.left + mono.left;
	sum.right = out1.right + out2.right + mono.right;

	//limiting
	sum.left = compressor_left_.processSample(out1.left + out2.left + mono.left);
	sum.right = compressor_right_.processSample(out1.right + out2.right + mono.left);

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

int AudioEngine::getEnergyState(float env)
{
	float scaledEnv = env * 5.0f;

	if (scaledEnv <= 0.24f)
	{
		return 0;
	}
	else if (scaledEnv <= 0.49f)
	{
		return 1;
	}
	else if (scaledEnv <= 0.74f)
	{
		return 2;
	}
	else
	{
		return 3;
	}
}

void AudioEngine::setOscGain(float gain1, float gain2, float gain3)
{
	oscGain_1 = gain1;
	oscGain_2 = gain2;
	oscGain_3 = gain3;
}

Compressor& AudioEngine::getCompressorLeft()
{
	return compressor_left_;
}

const Compressor& AudioEngine::getCompressorLeft() const
{
	return compressor_left_;
}

Compressor& AudioEngine::getCompressorRight()
{
	return compressor_right_;
}

const Compressor& AudioEngine::getCompressorRight() const
{
	return compressor_right_;
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


Panner& AudioEngine::getPanner_mono()
{
	return panner_mono;
}

const Panner& AudioEngine::getPanner_mono() const
{
	return panner_mono;
}

MarkovChain& AudioEngine::getMarkov_1()
{
	return markov_1;
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

Sequencer& AudioEngine::getSequencer_4()
{
	return sequencer_4;
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

Synthesizer& AudioEngine::getSynth_1()
{
	return synth_1;
}

Synthesizer& AudioEngine::getSynth_2()
{
	return synth_2;
}

Synthesizer& AudioEngine::getSynth_3()
{
	return synth_3;
}

Synthesizer& AudioEngine::getSynth_4()
{
	return synth_4;
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
