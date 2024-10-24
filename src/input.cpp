#include "input.hpp"
#include "window.hpp"

#include <windows.h>
#include <dinput.h>

static LPDIRECTINPUT8 di; 
static LPDIRECTINPUTDEVICE8 keyboard;
static BYTE keyboardData[256] = { 0 };
static BYTE keyboardDataOld[256] = { 0 };

bool Input::key[Actions::COUNT] = { false };
bool Input::keyPressed[Actions::COUNT] = { false };
bool Input::keyReleased[Actions::COUNT] = { false };

void Input::initialize()
{
	DirectInput8Create(Window::windowClass.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&di, NULL);
	di->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	keyboard->SetDataFormat(&c_dfDIKeyboard);
	keyboard->SetCooperativeLevel(Window::hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	keyboard->Acquire();
}

void Input::finalize()
{
	keyboard->Unacquire();
	keyboard->Release();
	di->Release();
}

void Input::update()
{
	memcpy(keyboardDataOld, keyboardData, sizeof(keyboardData));
	
	keyboard->GetDeviceState(sizeof(keyboardData), (LPVOID)&keyboardData);

	memset(keyPressed, 0, sizeof(keyPressed));
	memset(keyReleased, 0, sizeof(keyReleased));

	key[Actions::UP]    = keyboardData[DIK_W]      & 0x80;
	key[Actions::LEFT]  = keyboardData[DIK_A]      & 0x80;
	key[Actions::DOWN]  = keyboardData[DIK_S]      & 0x80;
	key[Actions::RIGHT] = keyboardData[DIK_D]      & 0x80;
	key[Actions::F11]   = keyboardData[DIK_F11]    & 0x80;
	key[Actions::ESC]   = keyboardData[DIK_ESCAPE] & 0x80;

	for (int i = 0; i < Actions::COUNT; ++i) {
		
		static int dikCode = 0;

		switch (static_cast<Actions>(i)) {

			case Actions::UP:    dikCode = DIK_W;      break;
			case Actions::LEFT:  dikCode = DIK_A;      break;
			case Actions::DOWN:  dikCode = DIK_S;      break;
			case Actions::RIGHT: dikCode = DIK_D;      break;
			case Actions::F11:   dikCode = DIK_F11;    break;
			case Actions::ESC:   dikCode = DIK_ESCAPE; break;
			default: continue;
		}

		keyPressed[i]  = (key[i] && !(keyboardDataOld[dikCode] & 0x80));
		keyReleased[i] = (!key[i] && (keyboardDataOld[dikCode] & 0x80));
	}
}

