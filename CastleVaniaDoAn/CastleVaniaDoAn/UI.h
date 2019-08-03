#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprite.h"
#include "Textures.h"
#include "Simon.h"

#define ID_TEX_UI_1 10
#define ID_TEX_UI_2 11
class CUI
{
private:
	ID3DXFont * g_font = NULL;
	HRESULT r = 0;
	RECT font_rect;

	wstring heart_number;

public:
	LPDIRECT3DDEVICE9 d3ddv;
	int score;
	int time;
	int stage;
	int player_heal;
	int enemy_heal;
	int heart;
	int life;
	int lastTick;
	int subWeapon;
	DWORD dt;
	vector<LPANIMATION> animations;
	CUI(LPDIRECT3DDEVICE9 d3ddv);


	void AddAnimation(int aniId);
	void Render();
	void Update(CSimon *simon);
	void DrawTextString(int x, int y, wchar_t *str);
};