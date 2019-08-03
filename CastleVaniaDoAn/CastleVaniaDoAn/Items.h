#pragma once
#include "GameObject.h"
#include "Define.h"



class CItems : public CGameObject
{
	int height;
	int width;
public:
	int item_type;
	void Render(float &cameraPosX, float &cameraPosY);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetSize(int width, int height) { this->width = width; this->height = height; };
	void Update(DWORD dt, vector<LPGAMEOBJECT> *ItemColiObjects);
	CItems(int itemType, float x, float y);
	~CItems();

	void Remove();
};

typedef CItems * LPCITEM;