// GenMusicAudioEngine.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include "AudioEngine.h"
#include "AudioOutput.h"
#include "Oscilloscope.h"
#include "portaudio.h"


struct AudioData
{
	AudioEngine engine;

	Oscilloscope oscilloscope;
};

static int audioCallback(const void* inputBuffer,
	void* outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo*,
	PaStreamCallbackFlags,
	void* userData)
{
	auto* data = static_cast<AudioData*>(userData);
	const float* in = static_cast<const float*>(inputBuffer);
	float* out = static_cast<float*>(outputBuffer);



	for (unsigned long i = 0; i < framesPerBuffer; i++)
	{
		float inputSample = 0.0f;

		if (in)
		{
			inputSample = in[i];
		}
		//float outputSample = data -> engine.processSample(inputSample);

		AudioOutput output = data->engine.processSample(inputSample);
		
		//out[i * 2 + 0] = outputSample;
		//out[i * 2 + 1] = outputSample;
		data->oscilloscope.pushSample(output.left, output.right);
		out[i * 2 + 0] = output.left ;
		out[i * 2 + 1] = output.right;
	}

	
	
	return paContinue;
}





int main()
{
	PaError err;
	PaStream* stream = nullptr;
	AudioData data;
	
	//prepare i gain
	data.engine.prepare(48000.0, 64);
	data.engine.setGain(1.0f);
	


	//data.engine.getPanner_ch1().setPan(0.0f);
	//data.engine.getPanner_ch2().setPan(1.0f);

	//noise
	data.engine.getWhiteNoiseGenerator().setGain(0.95f);

	//oscillator
	//data.engine.getOscillator().setFrequency(220.0f);
	data.engine.setOscGain(0.2f, 0.1f, 6.0f);

	//ustawienia lowpass filter
	data.engine.getLowPassFilter().setCutoffHz(2000.0f);
	data.engine.getLowPassFilter_2().setCutoffHz(2000.0f);
	//data.engine.getLowPassFilter_3().setCutoffHz(16000.0f);


	//ustawienia highpass filter
	data.engine.getHighPassFilter().setCutoffHz(12000.0f);

	//ustawienia delaya pierwszego
	data.engine.getDelay_1().setDelayTimeMs(100.0f);
	data.engine.getDelay_1().setFeedback(0.0f);
	data.engine.getDelay_1().setWetLevel(0.0f);

	//ustawienia delaya drugiego
	data.engine.getDelay_2().setDelayTimeMs(500.0f);
	data.engine.getDelay_2().setFeedback(0.85f);
	data.engine.getDelay_2().setWetLevel(0.90f);

	//ustawienia delaya trzeciego
	data.engine.getDelay_3().setDelayTimeMs(250.0f);
	data.engine.getDelay_3().setFeedback(0.96f);
	data.engine.getDelay_3().setWetLevel(1.0f);

	//ustawienia delaya czwartego
	data.engine.getDelay_4().setDelayTimeMs(1000.0f);
	data.engine.getDelay_4().setFeedback(0.98f);
	data.engine.getDelay_4().setWetLevel(1.0f);

	err = Pa_Initialize();
	if (err != paNoError)
	{
		std::cerr << "Pa_Initialize failed: " << Pa_GetErrorText(err) << "\n";
		return 1;
	}

	err = Pa_OpenDefaultStream(
		&stream,
		1,
		2,
		paFloat32,
		48000,
		64,
		audioCallback,
		&data
	);


	if (err != paNoError)
	{
		std::cerr << "Pa_OpenDefaultSystem failed: " << Pa_GetErrorText(err) << "\n";
		return 1;
	}

	err = Pa_StartStream(stream);
	if (err != paNoError)
	{
		std::cerr << "Pa_StartStream failed: " << Pa_GetErrorText(err) << "\n";
		Pa_CloseStream(stream);
		Pa_Terminate();
		return 1;

	}

	//oscilloscope
	std::atomic<bool> running = true;

	std::thread displayThread([&]()
		{
			while (running)
			{
				const auto buffer = data.oscilloscope.getSnapshot();
				char grid[20][80];

				system("cls");
				
				memset(grid, ' ', sizeof(grid));

				
				
				int firstLeftIndex = 0;
				int prevRow = std::clamp((int)((buffer[firstLeftIndex] + 1.0f) / 2.0f * 19), 0, 19);
				

				int firstRightIndex = 1; 
				int prevRowRight = std::clamp((int)((buffer[firstRightIndex] + 1.0f) / 2.0f * 19), 0, 19);
				
				std::cout << "INFINITE AMBIENT RUNNING...";
				for (int col = 0; col < 80; col++)
				{
					int sampleLeftIndex = col * (2048 / 80) &~1;
					int sampleRightIndex = sampleLeftIndex + 1;
					int row = std::clamp((int)((buffer[sampleLeftIndex] + 1.0f) / 2.0f * 19), 0, 19);
					

					int rowRight = std::clamp((int)((buffer[sampleRightIndex] + 1.0f) / 2.0f * 19), 0, 19);
					
					for (int r = std::min(prevRow, row); r <= std::max(prevRow, row); r++)
					{
						grid[r][col] = '*';
					}
					prevRow = row;

					for (int rr = std::min(prevRowRight, rowRight); rr <= std::max(prevRowRight, rowRight); rr++)
					{
						grid[rr][col] = '+';
					}
					prevRowRight = rowRight;

				}
				for (int row = 0; row < 20; row++)
				{
					std::cout.write(grid[row], 80); std::cout << "\n";
				}

				

			}
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
		});

	std::cout << "Audio running for ever... \n";
	
	std::this_thread::sleep_for(std::chrono::seconds(5000));

	running = false;

	displayThread.join();

	err = Pa_StopStream(stream);
	if (err != paNoError)
	{
		std::cerr << "Pa_StopStream failed: " << Pa_GetErrorText(err) << "\n";
	}

	Pa_CloseStream(stream);
	Pa_Terminate();


	return 0;
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
