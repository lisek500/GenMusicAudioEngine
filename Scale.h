#pragma once
#include "NotePitch.h"
#include "NoteValue.h"
#include <vector>


enum class Scale
{
	PENTATONIC_MINOR,
	PENTATONIC_MAJOR,
	MAJOR,
	MINOR,
	HARMONIC_MINOR,
	DORIAN,
	PHRYGIAN,
	LYDIAN,
	MIXOLYDIAN,
	LOCRIAN,
	WHOLETONE,

};

class ScaleClass
{
public:
	void setScale(Scale scale);

	void setRoot(Notes root, int octave);

	void rebuildScale();

	void updateIntervals();


	int getScaleSize() const;

	float getFrequencyFromScaleDegree(int degree) const;

private:

	Scale scale_ = Scale::PENTATONIC_MINOR;
	Notes root_ = Notes::A;
	int octave_ = 3;

	Notes indexToNote(int index) const;

	std::vector<int> intervals_;
	std::vector<float> frequencies_;

};
