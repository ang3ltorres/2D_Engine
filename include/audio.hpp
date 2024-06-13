#pragma once

#include <string>
#include <audioclient.h>

struct Sound
{
	std::string soundName;
	unsigned int totalSamples;
	unsigned char *data;
};

class Audio
{
public:
	Audio() = delete;
	~Audio() = default;

	static bool initialize();
	static bool finalize();

	static Sound loadSound(const std::string &fileName);

	static IAudioClient *pAudioClient;
	static IAudioRenderClient *pRenderClient;
};
