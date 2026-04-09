#include "NoteValue.h"

float NoteValueClass::noteDuration(NoteValue value)
{
	switch (value)
	{
	case NoteValue::WHOLE: return 1.0f;
	case NoteValue::HALF: return 0.5f;
	case NoteValue::QUARTER: return 0.25f;
	case NoteValue::EIGHTH: return 0.125f;
	case NoteValue::SIXTEENTH: return 0.0625;
	}
	return 1.0f;
}
