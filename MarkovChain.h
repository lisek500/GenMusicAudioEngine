#pragma once
#include <vector>
#include <random>
#include "Sequencer.h"

class MarkovChain
{
public:
	void prepare(int numStates, int numNotes, int numRhythms);

	void setState(int state);

	int getNextNote();

	NoteValue getNextDuration();

	void setNoteProbability(int fromState, int toNote, float probability);
	void setRhythmProbability(int fromState, int toNote, float probability);

private:
	int currentState_ = 0;
	int numStates_ = 0;
	int numNotes_ = 0;
	int numRhythms_ = 5;

	std::vector<std::vector<float>> noteMatrix_;
	std::vector<std::vector<float>> rhythmMatrix_;

	std::mt19937 gen_;

	int sampleFromDistribution(const std::vector<float>& probabilities);
};