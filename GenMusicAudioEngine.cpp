// GenMusicAudioEngine.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include <thread>
#include <chrono>
#include "AudioEngine.h"
#include "AudioOutput.h"
#include "portaudio.h"

struct AudioData
{
	AudioEngine engine;
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
	data.engine.getWhiteNoiseGenerator().setGain(0.75f);

	//oscillator
	//data.engine.getOscillator().setFrequency(220.0f);
	data.engine.setOscGain(0.1f, 0.1f, 6.0f);

	//ustawienia lowpass filter
	data.engine.getLowPassFilter().setCutoffHz(2000.0f);
	data.engine.getLowPassFilter_2().setCutoffHz(2000.0f);
	//data.engine.getLowPassFilter_3().setCutoffHz(16000.0f);


	//ustawienia highpass filter
	data.engine.getHighPassFilter().setCutoffHz(12000.0f);

	//ustawienia delaya pierwszego
	data.engine.getDelay_1().setDelayTimeMs(100.0f);
	data.engine.getDelay_1().setFeedback(0.1f);
	data.engine.getDelay_1().setWetLevel(0.15f);

	//ustawienia delaya drugiego
	data.engine.getDelay_2().setDelayTimeMs(135.0f);
	data.engine.getDelay_2().setFeedback(0.75f);
	data.engine.getDelay_2().setWetLevel(0.75f);

	//ustawienia delaya trzeciego
	data.engine.getDelay_3().setDelayTimeMs(135.0f);
	data.engine.getDelay_3().setFeedback(0.75f);
	data.engine.getDelay_3().setWetLevel(0.75f);

	//ustawienia delaya czwartego
	data.engine.getDelay_4().setDelayTimeMs(135.0f);
	data.engine.getDelay_4().setFeedback(0.95f);
	data.engine.getDelay_4().setWetLevel(0.95f);

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

	std::cout << "Audio running for ever... \n";
	
	std::this_thread::sleep_for(std::chrono::seconds(5000));

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
