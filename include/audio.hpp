#pragma once

#include <string>
#include <dsound.h>

class Audio 
{
public:
	static void initialize();
	static void finalize();
};

class Sound
{
public:
	Sound(const std::string &fileName);
	~Sound();
	void play();
	
	char *buffer;
	unsigned long bufferSize;
	LPDIRECTSOUNDBUFFER secondaryBuffer;
	DSBUFFERDESC secondaryBufferDes;
};
