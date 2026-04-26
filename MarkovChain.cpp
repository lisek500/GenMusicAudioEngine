#include "MarkovChain.h"

void MarkovChain::prepare(int numStates, int numNotes, int numRhythms)
{
	numStates_ = numStates;

	numNotes_ = numNotes;

	numRhythms_ = numRhythms;

	//random seeding
	std::random_device rd;
	gen_.seed(rd());


	//Note Matrix initial settings
	noteMatrix_.resize(numStates_, std::vector<float>(numNotes_));

	for (int i = 0; i < numStates_; i++)
	{
		for (int j = 0; j < numNotes_; j++)
		{
			noteMatrix_[i][j] = 1.0f / numNotes_;
		}

	}

	//Rhythm Matrix initial settings
	rhythmMatrix_.resize(numStates_, std::vector<float>(numRhythms_));

	for (int i = 0; i < numStates_; i++)
	{
		for (int j = 0; j < numRhythms_; j++)
		{
			rhythmMatrix_[i][j] = 1.0f / numRhythms_;
		}
	}


}

void MarkovChain::setState(int state)
{
	currentState_ = state;
}

int MarkovChain::sampleFromDistribution(const std::vector<float>& probabilities)
{
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);
	float randomValue = dist(gen_);

	float sum = 0.0f;
	for (int i = 0; i < probabilities.size(); i++)
	{
		sum += probabilities[i];

		if (randomValue < sum)
		{
			return i;
		}
	}
	return 0;
}


int MarkovChain::getNextNote()
{
	std::vector<float> probs = noteMatrix_[currentState_];
	return sampleFromDistribution(probs);

}
NoteValue MarkovChain::getNextDuration()
{
	std::vector<float> probs = rhythmMatrix_[currentState_];
	int index =  sampleFromDistribution(probs);
	return static_cast<NoteValue>(index);
}

void MarkovChain::setNoteProbability(int fromState, int toNote, float probability)
{
	noteMatrix_[fromState][toNote] = probability;
}

void MarkovChain::setRhythmProbability(int fromState, int toNote, float probability)
{
	rhythmMatrix_[fromState][toNote] = probability;
}

