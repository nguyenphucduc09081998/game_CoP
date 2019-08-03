#pragma once
#pragma once
#include"Enemy.h"

class CDog : public CEnemy
{
public:
	CDog(float x, float y, int nx, float distanceBeforeJump);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *CreepColiObjects, float SimonPosx, bool stopEnemy);
	void Render(float &cameraPosX, float &cameraPosY);
	~CDog();
};

