#pragma once
#include "GameObject.h"
#include "Simon.h"
#include "Items.h"
#include "Brick.h"

class CEnemy : public CGameObject
{

public:
	int height;
	int width;
	DWORD destroy_start;
	int _x;
	int itemDrop;
	float action;
	DWORD attackStart;
	bool canUpdate = true;
	bool canAttack = false;
	int damageDeal;
	int enemyType;
	bool stopEnemy = false;

	CEnemy() {};
	CEnemy(float x, float y, int nx);
	virtual void Render(float &cameraPosX, float &cameraPosY) = 0;
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *CreepColiObjects, float SimonPosx, bool stopEnemy) = 0;
	LPCITEM DropItem();
	void SetState(int state);
	void attack();
	~CEnemy();
};

