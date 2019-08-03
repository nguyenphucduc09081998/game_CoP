#pragma once
#include "GameObject.h"
#include "Mics.h"
#include "Boss.h"
class CSubWeapon : public CGameObject
{
	int weaponType;
	int width;
	int height;
	DWORD timeStart;
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT> *WeaponColiObjects);
	void Render(float & cameraPosX, float & cameraPosY);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Remove();
	CSubWeapon(int weaponType, int n);
	~CSubWeapon();
};

