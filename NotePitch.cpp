#include "NotePitch.h"
#include <cmath>


 float NotePitch::noteToFrequency(Notes notePitch, int octave)
{
	
	 int pitchIndex = static_cast<int>(notePitch);
	 
	 int noteNumber = octave * 12 + pitchIndex;

	 const int a4Number = 4 * 12 + 9;

	 int semitoneDistance = noteNumber - a4Number;
	 
	 return 440.f * std::pow(2.0f, semitoneDistance / 12.0f);
	 
	 
}

