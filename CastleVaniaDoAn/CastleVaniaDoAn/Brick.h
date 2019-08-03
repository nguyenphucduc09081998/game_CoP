#pragma once
#include "GameObject.h"
#include "Items.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class CBrick : public CGameObject
{
	float width;
	float height;
	int brickType;
public:
	float _x;
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	int item_drop;
	CBrick() : CGameObject()
	{
		type = 2;
		width = 16;
		height = 16;
		item_drop = 9;
	}

	CBrick(int brickType, float x, float y, int item_drop);
	void setSize(float width, float height) { this->width = width; this->height = height; };
	void SetState(int state);
	LPCITEM DropItem();
	virtual void Render(float &cameraPosX, float &cameraPosY);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};