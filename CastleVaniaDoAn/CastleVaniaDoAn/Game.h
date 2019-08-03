#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

#define DIRECTINPUT_VERSION 0x0800
#define KEYBOARD_BUFFER_SIZE 1024

class CKeyEventHandler
{
public:
	virtual void KeyState(BYTE *state) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
};
typedef CKeyEventHandler * LPKEYEVENTHANDLER;

class CGame
{
	static CGame * __instance;

	HWND hWnd;

	LPDIRECT3D9 d3d = NULL;
	LPDIRECT3DDEVICE9 d3ddv = NULL;

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;

	LPDIRECTINPUT8       di;
	LPDIRECTINPUTDEVICE8 didv;

	BYTE  keyStates[256];
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];

	LPKEYEVENTHANDLER keyHandler;

public:
	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void InitKeyboard(LPKEYEVENTHANDLER handler);
	void Init(HWND hWnd);

	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);

	static void SweptAABB(
		float ml,
		float mt,
		float mr,
		float mb,
		float dx,
		float dy,
		float sl,
		float st,
		float sr,
		float sb,
		float &t,
		float &nx,
		float &ny);

	static CGame * GetInstance();

	~CGame();
};

