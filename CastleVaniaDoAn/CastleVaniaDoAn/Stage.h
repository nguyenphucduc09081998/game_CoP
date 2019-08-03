#pragma once
#pragma once
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Sprite.h"
#include "Simon.h"
#include "Brick.h"
#include "BackGround.h"
#include "MorningStar.h"
#include "Mics.h"
#include "SubWeapon.h"
#include "Define.h"
#include "Stair.h"
#include "Enemy.h"
#include "Effect.h"
#include "Boss.h"
#include "Zombie.h"
#include "Dog.h"
#include "Bat.h"
#include "Fish.h"
class CStage
{

	int stage;
	vector<LPGAMEOBJECT> SimonColiObjects;
	vector<LPGAMEOBJECT> CreepColiObjects;
	vector<LPGAMEOBJECT> MicsColiObjects;
	vector<vector<int>> ZonesObjects;
	vector<LPGAMEOBJECT> WeaponColiObjects;
	vector<LPGAMEOBJECT> ItemColiObjects;
	vector<LPGAMEOBJECT> SubWeapon_objects;
	vector<int> SpecialObjects; // gate, ladder
	CSimon *Simon;
	CMorningStar *MorningStar;
	CBackGround *BackGround;
	vector<LPCSTAIR> stairs;
	int currentZone;
	int stageWidth;
	int noZone;
	bool stopEnemy = false;
	DWORD stopEnemyStart;

public:
	bool DestroyEnemy = false;
	int bossHeal = 16;
	int flag1 = 0;
	bool ended;
	int nextStage;
	CStage(
		int stage,
		vector<LPGAMEOBJECT> allGameObject,
		vector<int> stageObjects,
		vector<LPGAMEOBJECT> SimonColiObjects,
		vector<LPGAMEOBJECT> CreepColiObjects,
		vector<LPGAMEOBJECT> MicsColiObjects,
		vector<LPGAMEOBJECT> WeaponColiObjects,
		vector<LPGAMEOBJECT> ItemColiObjects,
		CSimon *Simon,
		CMorningStar *MorningStar,
		//CBackGround *BackGround,
		CBackGround *BackGround,
		int stageWidth,
		vector<LPCSTAIR> stairs
	);
	void Update(DWORD dt, vector<LPGAMEOBJECT> &allGameObject, float &cameraPosX, float &cameraPosY, int& debug);
	void Render(float cameraPosX, float cameraPosY, vector<LPGAMEOBJECT> allGameObject);
	void Insert_gameObject(LPGAMEOBJECT go, vector<LPGAMEOBJECT> &allGameObject);
	~CStage();

};

typedef CStage * LPSTAGE;