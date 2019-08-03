#pragma once
#include "GameObject.h"
#include "MorningStar.h"
#include "Define.h"
#include "Stair.h"
#include "Enemy.h"
#include "Effect.h"

class CSimon : public CGameObject
{
	int posXforStair = 0;
	int posYforStair = 0;
	int tempXforStair = 0;
	int tempYforStair = 0;


	DWORD untouchable_start;
	DWORD attack_start;
	DWORD lastStepTimeStart;

public:
	bool destroyEnemy = false;
	bool stopCamera = false;
	LPCSTAIR currentStair;
	int untouchable;
	bool isLastFrame;
	bool nearUpStair;
	bool nearDownStair;
	bool onStair = false;
	int action;
	int attacking;
	int player_heal;
	int heart;
	int subWeapon;
	int created_subweapon;
	CSimon() : CGameObject()
	{
		type = 1;
		untouchable = 0;
		attacking = 0;
		heart = 5;
		player_heal = 16;
		isLastFrame = 0;

		SetState(SIMON_STATE_IDLE);
		AddAnimation(100);
		AddAnimation(101);
		AddAnimation(102);
		AddAnimation(103);
		AddAnimation(104);
		AddAnimation(105);
		AddAnimation(106);
		AddAnimation(107);
		AddAnimation(108);
		AddAnimation(109);
		AddAnimation(110);
		AddAnimation(111);
		AddAnimation(112);
		AddAnimation(113);
		AddAnimation(114);
		AddAnimation(115);
		AddAnimation(116);
		AddAnimation(117);
		AddAnimation(118);
		AddAnimation(119);
		AddAnimation(120);
		AddAnimation(121);
		AddAnimation(122);
		AddAnimation(123);
		SetPosition(50.0f, 150);
	}

	void Update(DWORD dt, float &cameraPosX, float &cameraPosY, int stage_width, CMorningStar* morningStar, vector<LPGAMEOBJECT> *colliable_objects, vector<LPCSTAIR> Stairs, int& score);
	void Render(float &cameraPosX, float &cameraPosY);
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void Attack(CMorningStar* morningStar, float cameraPosX, float cameraPosY, int attackType);
	void GoOnStair();
	int CurrentStairDirection() { return currentStair->nx; };
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void takeDame(int nx);
};