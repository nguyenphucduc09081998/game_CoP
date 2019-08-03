#pragma once
#include"Enemy.h"
class CFish : public CEnemy
{
public:
	CFish(float x, float y, int nx);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *CreepColiObjects, float SimonPosx, bool stopEnemy);
	void Render(float &cameraPosX, float &cameraPosY);
	~CFish();
};

#pragma once
