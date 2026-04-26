#pragma once

struct AudioOutput
{
	float left = 0.0f;
	float right = 0.0f;

	AudioOutput operator*(float gain) const
	{
		AudioOutput result;
		result.left = left * gain;
		result.right = right * gain;

		return result;
	}

	AudioOutput operator+(const AudioOutput& other) const
	{
		AudioOutput result;
		result.left = left + other.left;
		result.right = right + other.right;

		return result;
	}
};