#pragma once

#include <string>

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
	
	unsigned long bufferSize;
	char *bufferData;
	void *buffer;
};
