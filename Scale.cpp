#include "Scale.h"

void ScaleClass::setScale(Scale scale)
{
	scale_ = scale;

	rebuildScale();

}

void ScaleClass::setRoot(Notes root, int octave)
{
	root_ = root;

	octave_ = octave;

	rebuildScale();
}

void ScaleClass::updateIntervals()
{
	switch (scale_)
	{
	case Scale::PENTATONIC_MINOR: 
			intervals_.push_back(0),
			intervals_.push_back(3),
			intervals_.push_back(5),
			intervals_.push_back(7),
			intervals_.push_back(10);
			
			break;

	case Scale::PENTATONIC_MAJOR:
			intervals_.push_back(0),
			intervals_.push_back(2),
			intervals_.push_back(5),
			intervals_.push_back(7),
			intervals_.push_back(9);

			break;

	case Scale::MAJOR:
			intervals_.push_back(0),
			intervals_.push_back(2),
			intervals_.push_back(4),
			intervals_.push_back(5),
			intervals_.push_back(7),
			intervals_.push_back(9),
			intervals_.push_back(11);

			break;

	case Scale::DORIAN:
			intervals_.push_back(0),
			intervals_.push_back(2),
			intervals_.push_back(3),
			intervals_.push_back(5),
			intervals_.push_back(7),
			intervals_.push_back(9),
			intervals_.push_back(10);

			break;

	case Scale::PHRYGIAN:
			intervals_.push_back(0),
			intervals_.push_back(1),
			intervals_.push_back(3),
			intervals_.push_back(5),
			intervals_.push_back(7),
			intervals_.push_back(8),
			intervals_.push_back(10);

			break;

	case Scale::LYDIAN:
			intervals_.push_back(0),
			intervals_.push_back(2),
			intervals_.push_back(4),
			intervals_.push_back(6),
			intervals_.push_back(7),
			intervals_.push_back(9),
			intervals_.push_back(11);

			break;

	case Scale::MIXOLYDIAN:
			intervals_.push_back(0),
			intervals_.push_back(2),
			intervals_.push_back(4),
			intervals_.push_back(5),
			intervals_.push_back(7),
			intervals_.push_back(9),
			intervals_.push_back(10);
			
			break;

	case Scale::MINOR:
			intervals_.push_back(0),
			intervals_.push_back(2),
			intervals_.push_back(3),
			intervals_.push_back(5),
			intervals_.push_back(7),
			intervals_.push_back(8),
			intervals_.push_back(10);

			break;

	case Scale::HARMONIC_MINOR:
			intervals_.push_back(0),
			intervals_.push_back(2),
			intervals_.push_back(3),
			intervals_.push_back(5),
			intervals_.push_back(7),
			intervals_.push_back(8),
			intervals_.push_back(11);

			break;

	case Scale::LOCRIAN:
			intervals_.push_back(0),
			intervals_.push_back(1),
			intervals_.push_back(3),
			intervals_.push_back(6),
			intervals_.push_back(7),
			intervals_.push_back(8),
			intervals_.push_back(10);

			break;

	case Scale::WHOLETONE:
			intervals_.push_back(0),
			intervals_.push_back(2),
			intervals_.push_back(4),
			intervals_.push_back(6),
			intervals_.push_back(8),
			intervals_.push_back(10);

		break;
			

	}
}

Notes ScaleClass::indexToNote(int index) const
{
	switch (index)
	{
	case 0: return Notes::C;
	case 1: return Notes::Db;
	case 2: return Notes::D;
	case 3: return Notes::Eb;
	case 4: return Notes::E;
	case 5: return Notes::F;
	case 6: return Notes::Fs;
	case 7: return Notes::G;
	case 8: return Notes::Ab;
	case 9: return Notes::A;
	case 10: return Notes::Bb;
	case 11: return Notes::B;
	}
		
	return Notes::A;
}

int ScaleClass::getScaleSize() const
{
	return frequencies_.size();
}

float ScaleClass::getFrequencyFromScaleDegree(int degree) const
{
	if (frequencies_.empty())
	{
		return 440.0f;
	}
	
	int scaleSize = getScaleSize();
	
	int wrappedIndex = degree % scaleSize;

	if (wrappedIndex < 0)
	{
		wrappedIndex += scaleSize;

	}



		return frequencies_[wrappedIndex];
	
	
}

void ScaleClass::rebuildScale()
{
	intervals_.clear();
	frequencies_.clear();

	updateIntervals();

	
	int rootIndex = static_cast<int>(root_);

	for (int interval : intervals_)
	{
		int total = rootIndex + interval;

		 int newPitchIndex = total % 12;

		 int octaveOffset = total / 12;

		 int newOctave = octave_ + octaveOffset;

		 Notes newNote = indexToNote(newPitchIndex);

		float frequency = NotePitch::noteToFrequency(newNote, newOctave);
		 
		frequencies_.push_back(frequency);
		 
	}





}