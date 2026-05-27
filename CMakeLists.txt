cmake_minimum_required(VERSION 3.20)
project(GenMusicAudioEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(GenMusicAudioEngine
    GenMusicAudioEngine.cpp
    Compressor.cpp
    Synthesizer.cpp
    MarkovChain.cpp
    ADSR.cpp
 	AudioEngine.cpp
	Delay.cpp
	EnvFollower.cpp
	HighPassFilter.cpp
	LFO.cpp
	LowPassFilter.cpp
	NotePitch.cpp
	NoteValue.cpp
	Oscillator.cpp
	Oscilloscope.cpp
	Panner.cpp
	Scale.cpp
	Sequencer.cpp
	WhiteNoiseGenerator.cpp
)   

target_include_directories(GenMusicAudioEngine PRIVATE .)


if(APPLE)
    target_include_directories(GenMusicAudioEngine PRIVATE /opt/homebrew/include)
    target_link_directories(GenMusicAudioEngine PRIVATE /opt/homebrew/lib)
    target_link_libraries(GenMusicAudioEngine PRIVATE portaudio)
elseif(WIN32)
    target_include_directories(GenMusicAudioEngine PRIVATE "C:/Users/mati5/Desktop/Programming/Libs/portaudio/include")
    target_link_libraries(GenMusicAudioEngine PRIVATE "C:/Users/mati5/Desktop/Programming/Libs/portaudio-build/Release/portaudio_static_x64.lib")
elseif(UNIX)
    target_include_directories(GenMusicAudioEngine PRIVATE /usr/include)
    target_link_directories(GenMusicAudioEngine PRIVATE /usr/lib)
    target_link_libraries(GenMusicAudioEngine PRIVATE portaudio)
endif()
