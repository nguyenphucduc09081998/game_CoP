#pragma once
#include"Enemy.h"
class CBat : public CEnemy
{
public:
	CBat(float x, float y, int nx);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *CreepColiObjects, float SimonPosx, bool stopEnemy);
	void Render(float &cameraPosX, float &cameraPosY);
	~CBat();
};

