#include "audio.hpp"

#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>

SoLoud::Soloud soloud;

void Audio::initialize() {
    soloud.init(SoLoud::Soloud::CLIP_ROUNDOFF, SoLoud::Soloud::MINIAUDIO);
}

void Audio::finalize() {
    soloud.deinit();
}

Sound::Sound(const std::string &fileName)
{
	wav = (void*) new SoLoud::Wav();
	((SoLoud::Wav*)wav)->load(fileName.c_str());
}

Sound::~Sound()
{
	((SoLoud::Wav*)wav)->stop();
}

void Sound::play()
{
	soloud.play(*((SoLoud::Wav*)wav));
}