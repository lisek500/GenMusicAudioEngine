#pragma once

enum class Notes
{
	C = 0,
	Db = 1,
	D = 2,
	Eb = 3,
	E = 4,
	F = 5,
	Fs = 6,
	G = 7,
	Ab = 8,
	A = 9,
	Bb = 10,
	B = 11,
	

	

};

struct Note
{
	Notes note;
	int octave;



};

class NotePitch
{
public:
	static float noteToFrequency(Notes notePitch, int octave);



};