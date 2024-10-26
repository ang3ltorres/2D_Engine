#include "audio.hpp"
#include "window.hpp"

#include <windows.h>
#include <dsound.h>
#include <string>
#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>

#define secondaryBuffer (*(LPDIRECTSOUNDBUFFER*)buffer)

static LPDIRECTSOUND dsound = nullptr;
static LPDIRECTSOUNDBUFFER primaryBuffer = nullptr;
static WAVEFORMATEX waveFormat;
static DSBUFFERDESC primaryBufferDesc;

void Audio::initialize()
{
	DirectSoundCreate(nullptr, &dsound, nullptr);
	dsound->SetCooperativeLevel(Window::hwnd, DSSCL_PRIORITY);

	ZeroMemory(&waveFormat, sizeof(WAVEFORMATEX));
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nChannels = 2;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nBlockAlign = (waveFormat.nChannels * waveFormat.wBitsPerSample) / 8;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;

	ZeroMemory(&primaryBufferDesc, sizeof(DSBUFFERDESC));
	primaryBufferDesc.dwSize = sizeof(primaryBufferDesc);
	primaryBufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	dsound->CreateSoundBuffer(&primaryBufferDesc, &primaryBuffer, nullptr);
	primaryBuffer->SetFormat(&waveFormat);
}

void Audio::finalize()
{
	primaryBuffer->Release();
	dsound->Release();
}

Sound::Sound(const std::string &fileName)
{
	buffer = new LPDIRECTSOUNDBUFFER{};

	OggVorbis_File vorbisFile;
	ov_fopen(fileName.c_str(), &vorbisFile);
	bufferSize = ov_pcm_total(&vorbisFile, -1) * waveFormat.nBlockAlign;

	DSBUFFERDESC secondaryBufferDes;
	ZeroMemory(&secondaryBufferDes, sizeof(DSBUFFERDESC));
	secondaryBufferDes.dwSize = sizeof(DSBUFFERDESC);
	secondaryBufferDes.dwFlags = DSBCAPS_CTRLDEFAULT;
	secondaryBufferDes.dwBufferBytes = bufferSize;
	secondaryBufferDes.lpwfxFormat = &waveFormat;

	dsound->CreateSoundBuffer(&secondaryBufferDes, &secondaryBuffer, nullptr);
	secondaryBuffer->SetFormat(&waveFormat);
	secondaryBuffer->Lock(0, bufferSize, (LPVOID*)&bufferData, &bufferSize, nullptr, 0, 0);

	unsigned long bytesRead;
	unsigned long totalRead = 0;

	while (totalRead < bufferSize) {

		bytesRead = ov_read(&vorbisFile, bufferData + totalRead, bufferSize - totalRead, 0, 2, 1, nullptr);
		if (bytesRead <= 0)
			break;

		totalRead += bytesRead;
	}
	secondaryBuffer->Unlock(bufferData, totalRead, nullptr, 0);
	ov_clear(&vorbisFile);
	preload();
}

Sound::~Sound()
{
	secondaryBuffer->Release();
}

void Sound::preload()
{
	secondaryBuffer->Play(0, 0, 0);
	secondaryBuffer->SetCurrentPosition(0);
	secondaryBuffer->Stop();
}

void Sound::play()
{
	secondaryBuffer->SetCurrentPosition(0);
	secondaryBuffer->Play(0, 0, 0);

	// DWORD status;
	// do {
	// 	secondaryBuffer->GetStatus(&status);
	// 	Sleep(100);	
	// } while(status & DSBSTATUS_PLAYING);
}