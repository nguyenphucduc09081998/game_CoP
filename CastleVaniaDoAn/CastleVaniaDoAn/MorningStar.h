#pragma once
#pragma once
#include "GameObject.h"
#include "Mics.h"
#include "Brick.h"


#define MS_STATE_LV1 100
#define MS_STATE_LV2 200
#define MS_STATE_LV3 300

#define MS_ANI_LV1_LEFT 0
#define MS_ANI_LV1_RIGHT 1
#define MS_ANI_LV2_LEFT 2
#define MS_ANI_LV2_RIGHT 3
#define MS_ANI_LV3_LEFT 4
#define MS_ANI_LV3_RIGHT 5

#define MS_TIME_TO_DISPLAY_LAST_FRAME 300
#define MS_TOTAL_ATTACK_TIME 450				// đây phải bằng với attack time bên simon và bằng tổng các time các frame cộng lại

class CMorningStar : public CGameObject
{
	DWORD attack_start;
	int height;
	int width;

	int xModifyL;
	int xModifyR;
public:
	int is_last_frame = 0;
	CMorningStar() : CGameObject()
	{
		type = 4;
		state = MS_STATE_LV1;
		height = 15;
		width = 28;
		xModifyL = 27;
		xModifyR = 11;
		AddAnimation(400);
		AddAnimation(401);
		AddAnimation(402);
		AddAnimation(403);
		AddAnimation(404);
		AddAnimation(405);
	}
	void Update(DWORD dt, float simonPosX, float simonPosY, int nx_simon, vector<LPGAMEOBJECT> *WeaponColiObjects);
	void Render(float cameraPosX, float cameraPosY, int nx_simon);
	void Render(float &cameraPosX, float &cameraPosY);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void isColidingObjects(vector<LPGAMEOBJECT>* coObjects, vector<LPGAMEOBJECT> &colidingObjects);
	void upgrade();
};