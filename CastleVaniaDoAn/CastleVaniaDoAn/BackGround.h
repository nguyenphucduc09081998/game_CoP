#pragma once
#include "GameObject.h"
#include "Define.h"


class CBackGround
{
	vector<LPSPRITE> sprites;
	vector<vector<int>> position;
	float x;
	float y;
public:
	CBackGround(vector<LPSPRITE> sprites, vector<vector<int>> position, float x, float y)
	{
		this->sprites = sprites;
		this->position = position;
		this->x = x;
		this->y = y;
	}
	void Render(float cameraPosX, float cameraPosY);
};



