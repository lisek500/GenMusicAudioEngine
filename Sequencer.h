#pragma once
#include "NotePitch.h"
#include "NoteValue.h"
#include "Scale.h"


struct Step
{
	int scaleDegree = 0;
	NoteValue duration;
	
	
};

class Sequencer
{
public:

	void prepare(double sampleRate,float energyThreshold, double BPM);

	void update(float energy);

	Step getCurrentStep();

	int setStepCount(int stepCount);

	void setStep(int index, int scaleDegree, NoteValue duration);


	void generateRandomSteps(int stepCount, int scaleSize, NoteValue duration);


	void setSteps(Step newSteps[], int stepCount);

	bool bConsumeLoopWrapped(bool bHasPendingUpdate);


private:

	void nextStep();

	double sampleRate_ = 48000.0;

	double BPM_ =  60.0;

	int currentStep_;

	double sampleCounter_;

	double stepDurationSamples_;

	Step steps_[16];

	int stepCount_;

	int barCount_ = 0;

	bool bLoopWrapped_ = true;

	bool bHasPendingUpdate_ = false;

	float energyThreshold_ = 0.0f;
};
