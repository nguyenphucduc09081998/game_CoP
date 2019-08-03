#pragma once
#include "GameObject.h"
#include "Items.h"


#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 32


class CMics : public CGameObject
{
	DWORD destroy_start;
	int itemDrop;
	int width;
	int height;
public:
	int _x;
	CMics(int micsType, int itemDrop, float x, float y);
	void Render(float &cameraPosX, float &cameraPosY);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetState(int state);
	LPCITEM DropItem();
	~CMics() {};
};
typedef CMics * LPCMIC;