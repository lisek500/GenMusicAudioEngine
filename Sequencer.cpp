#include "Sequencer.h"
#include "iostream"
#include <ctime>
#include <algorithm>


void Sequencer::prepare(double sampleRate,float energyThreshold, double BPM)
{
	sampleRate_ = sampleRate;

	BPM_ = BPM;

	currentStep_ = 0;

	sampleCounter_ = 0.0;

	bLoopWrapped_ = false;

	std::srand(std::time({}));
}

bool Sequencer::bConsumeLoopWrapped(bool bHasPendingUpdate)
{
	bHasPendingUpdate_ = bHasPendingUpdate;

	if (bHasPendingUpdate_)
	{
		if (bLoopWrapped_)
		{
			bLoopWrapped_ = false;
			//std::srand(std::time({}));
			std::cout << "loop changed";
			return true;

		

		}
		return false;
	}

	return false;
}

void Sequencer::nextStep()
{
	bLoopWrapped_ = false;

	currentStep_++;

	if (currentStep_ >= stepCount_)
	{
		currentStep_ = 0;
		bLoopWrapped_ = true;
	}

	
	stepDurationSamples_ = NoteValueClass::noteDuration(steps_[currentStep_].duration) * (4.0 * 60.0 / BPM_) * sampleRate_;
	
	std::cout << currentStep_;
}

void Sequencer::update(float energy)
{
	sampleCounter_++;

	if (sampleCounter_ >= stepDurationSamples_)
	{
		nextStep();
		sampleCounter_ = 0.0;
	}


	
}

Step Sequencer::getCurrentStep()
{
	return steps_[currentStep_];
}

int Sequencer::setStepCount(int stepCount)
{
	if (stepCount < 1)
	{
		stepCount_ = 1;
	}

	else if (stepCount > 16)
	{
		stepCount_ = 16;
	}
	else
	{
		stepCount_ = stepCount;
	}
	 
	return stepCount_;
}

void Sequencer::setStep(int index, int scaleDegree, NoteValue duration)
{
	if (index < 0 || index > 15)
	{
		return;
	}

	steps_[index].scaleDegree = scaleDegree;
	steps_[index].duration = duration;

}

void Sequencer::generateRandomSteps(int stepCount, int scaleSize, NoteValue duration)
{
	bLoopWrapped_ = false;

	setStepCount(stepCount);

	if (scaleSize <= 0)
	{
		return;
	}



	for (int i = 0; i < stepCount_; i++)
	{
		int rand = std::rand() % scaleSize;
		setStep(i, rand, duration);

		
	}

	currentStep_ = 0;

	sampleCounter_ = 0.0;

	stepDurationSamples_ = NoteValueClass::noteDuration(steps_[currentStep_].duration) * (4.0 * 60.0 / BPM_) * sampleRate_;

	

}

void Sequencer::setSteps(Step newSteps[], int stepCount)
{
	for (int i = 0; i < stepCount; i++)
	{
		steps_[i] = newSteps[i];
	}
	
	stepCount_ = stepCount;

}
