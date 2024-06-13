#include "audio.hpp"

#include <mmdeviceapi.h>

IAudioClient *Audio::pAudioClient = nullptr;
IAudioRenderClient *Audio::pRenderClient = nullptr;

bool Audio::initialize()
{
	IMMDeviceEnumerator *pEnumerator = nullptr;
	IMMDevice *pDevice = nullptr;

	// Initialize COM
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

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

	return true;
}

bool Audio::finalize()
{
	pRenderClient->Release();
	pAudioClient->Release();
}

Sound loadSound(const std::string &fileName)
{
	
}

