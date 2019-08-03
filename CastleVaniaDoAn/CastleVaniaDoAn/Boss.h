#pragma once
#include "GameObject.h"
#include "Simon.h"

class CBoss : public CGameObject
{
public:
	bool flipYFlag = false;
	float tmpSimonX;
	float tmpSimonY;
	float tmpX;
	int width;
	int height;
	int totalMovingDistance;
	DWORD actionTime;
	DWORD takeDamageCoolDown;
	DWORD attackStart;
	bool canAttack;
	int bossHeal;
	bool stopEnemy = false;
	CBoss(float x, float y);
	~CBoss();

	void Render(float &cameraPosX, float &cameraPosY);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Update(DWORD dt, CSimon *Simon, bool stopEnemy);
	void TakeDamage(int damage);
};

