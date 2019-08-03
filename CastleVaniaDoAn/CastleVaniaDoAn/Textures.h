#pragma once
#include <unordered_map>
#include <d3dx9.h>
#include <Windows.h>
#include <d3d9.h>
#include "Game.h"


using namespace std;

class CTextures
{

	unordered_map<int, LPDIRECT3DTEXTURE9> textures;

public:
	static CTextures * __instance;
	CTextures();
	void Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 Get(unsigned int i);
	static CTextures * GetInstance();

};