#pragma once
#include "GameObject.h"

class CEffect : public CGameObject
{
public:
	DWORD destroy_start;
	CEffect(int effecttype, float x, float y, float vx, float vy, int flag);
	CEffect(float x, float y, float vx, float vy, int nx);
	~CEffect();

	void Render(float &cameraPosX, float &cameraPosY);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Update(DWORD dt);
	void SetState(int state);
};

typedef CEffect * LPCEFFECT;
