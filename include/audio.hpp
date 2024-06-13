#pragma once

#include <string>

struct Sound
{
	std::string soundName;
	long totalSamples;
	char *data;
};

class Audio
{
public:
	Audio() = delete;
	~Audio() = default;

	static bool initialize();
	static bool finalize();

	static Sound loadSound(const std::string &fileName);
};
