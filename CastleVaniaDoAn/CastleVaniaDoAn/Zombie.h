#pragma once
#include"Enemy.h"

class CZombie : public CEnemy
{
public:
	CZombie(float x, float y, int nx);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *CreepColiObjects, float SimonPosx, bool stopEnemy);
	void Render(float &cameraPosX, float &cameraPosY);
	~CZombie();
};

