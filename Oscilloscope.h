#pragma once
#include <vector>
#include <atomic>
#include <array>


class Oscilloscope
{
public:
	void prepare();
	
	void pushSample(float sampleLeft,float sampleRight);

	const std::array<float, 2048>& getSnapshot() const;

private:
	std::array<float, 2048> buffer_;

	std::atomic<int> writeIndex_;

	std::atomic<int> readIndex_;

	



};