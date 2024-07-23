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

	static void initialize();
	static void finalize();

	static Sound loadSound(const std::string &fileName);
};
