#pragma once

enum class NoteValue
{
	WHOLE,
	HALF,
	QUARTER,
	EIGHTH,
	SIXTEENTH,

};

class NoteValueClass
{
public:
	static float noteDuration(NoteValue value);
};