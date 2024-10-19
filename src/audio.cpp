#include "audio.hpp"

#include <cstdio>
#include <vorbis/vorbisfile.h>
#include <ogg/ogg.h>
#include <mmdeviceapi.h>
#include <audioclient.h>

IAudioClient *pAudioClient = nullptr;
IAudioRenderClient *pRenderClient = nullptr;

void Audio::initialize()
{
	IMMDeviceEnumerator *pEnumerator = nullptr;
	IMMDevice *pDevice = nullptr;

	// Initialize the windows WASAPI API
	CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
	pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
	pEnumerator->Release();
	pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (LPVOID*)pAudioClient);
	pDevice->Release();

	// Initialize the audio client
	WAVEFORMATEX *pwfx = new WAVEFORMATEX();
	pwfx->wFormatTag = WAVE_FORMAT_PCM;
	pwfx->nChannels = 2;
	pwfx->nSamplesPerSec = 44100;
	pwfx->nAvgBytesPerSec = 44100 * 2 * 2; // 2 channels * 16 bits per sample
	pwfx->nBlockAlign = 4;
	pwfx->wBitsPerSample = 16;
	pwfx->cbSize = 0;

	pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 10000000, 0, pwfx, nullptr);

	// Get the audio render client
	pAudioClient->GetService(__uuidof(IAudioRenderClient), (void**)&pRenderClient);
}

void Audio::finalize()
{
	pRenderClient->Release();
	pAudioClient->Release();
}

Sound loadSound(const std::string &fileName)
{
	OggVorbis_File oggFile;
	FILE *file;
	char *buffer;

	file = fopen(fileName.c_str(), "rb");
	ov_open(file, &oggFile, NULL, 0);

	long totalSamples = ov_pcm_total(&oggFile, -1);
	buffer = (char*)malloc(totalSamples * sizeof(float));
	ov_read(&oggFile, buffer, totalSamples * sizeof(float), 0, 2, 1, nullptr);

	ov_clear(&oggFile);
	fclose(file);

	return Sound
	{
		fileName,
		totalSamples,
		buffer
	};
}
