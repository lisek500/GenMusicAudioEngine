#include "Oscilloscope.h"

void Oscilloscope::prepare()
{
	writeIndex_ = 0;

	//readIndex_ = 0;
}

void Oscilloscope::pushSample(float sampleLeft, float sampleRight)
{
	buffer_[writeIndex_] = sampleLeft;

	buffer_[writeIndex_ + 1] =  sampleRight;

	writeIndex_ = (writeIndex_ + 2) % 2048;
}

std::array<float, 2048> Oscilloscope::getSnapshot() const
{
	return buffer_;
}