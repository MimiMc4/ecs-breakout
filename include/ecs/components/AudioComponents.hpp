#pragma once

#include <string>

struct Sound {
	std::string name;
	float pitch = 1;
	float volume = 1;

	bool play = false;
	bool pitchChanged = false;
	bool volumeChanged = false;

};
