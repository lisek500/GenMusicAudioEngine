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

	return output;
}

AudioOutput Panner::processSample(AudioOutput input)
{
	AudioOutput output;

	output.left = input.left * cos(pan_ * pi / 2) + input.right * sin((1-pan_) * pi / 2);
	
	output.right = input.right * cos(pan_ * pi / 2) + input.left * sin((2 - pan_) * pi / 2);


		return output;

}

