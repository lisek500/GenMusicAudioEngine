#include "Panner.h"
#include <cmath>
#include <algorithm>


void Panner::setPan(float pan)
{

	pan_ = std::clamp(pan, 0.0f, 1.0f);

}

AudioOutput Panner::processSample(float input)
{
	AudioOutput output;

	output.left = input * cos(pan_ * pi / 2);
	
	output.right = input * sin(pan_ * pi / 2); 

	output.mono = input;

		return output;

}

