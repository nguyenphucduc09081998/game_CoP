#include "MorningStar.h"

void CMorningStar::Update(DWORD dt, float simonPosX, float simonPosY, int nx_simon, vector<LPGAMEOBJECT> *WeaponColiObjects)
{
	this->dt = dt;
	nx = nx_simon;

	if (nx_simon > 0) {
		this->x = simonPosX - xModifyR;
		this->y = simonPosY;
	}
	else {
		this->x = simonPosX - xModifyL;
		this->y = simonPosY;

	}



	if (attack_start == 0) {
		attack_start = GetTickCount();
	}
	else {
		if (GetTickCount() - attack_start < MS_TOTAL_ATTACK_TIME) {
			if (GetTickCount() - attack_start > MS_TIME_TO_DISPLAY_LAST_FRAME) {
				is_last_frame = 1;

			}
			else {
				is_last_frame = 0;
			}
		}
		else {
			attack_start = 0;
		}
	}

	if (is_last_frame) {
		vector<LPGAMEOBJECT> colidingObjects;
		this->isColidingObjects(WeaponColiObjects, colidingObjects);
		for (UINT i = 0; i < colidingObjects.size(); i++) {
			CBrick * brick = NULL;
			switch (colidingObjects[i]->type) { 
				case GO_TYPE_MICS:
				case GO_TYPE_ENEMY:
				case GO_TYPE_EFFECT_BULLET:
					colidingObjects[i]->SetState(STATE_BEING_REMOVE);
					break;
				case GO_TYPE_BRICK:
					brick = dynamic_cast<CBrick*>(colidingObjects[i]);
					if (brick->item_drop != BRICK_UNABLE_DESTROY) {
						brick->SetState(STATE_REMOVED);
					}
					break; 
				case GO_TYPE_BOSS:
					colidingObjects[i]->TakeDamage(2);
					break;
				
			}
		}
	}
}

void CMorningStar::Render(float cameraPosX, float cameraPosY, int nx_simon)
{
	int ani;
	switch (state) {
	case MS_STATE_LV1:		
		if (nx_simon>0) ani = MS_ANI_LV1_RIGHT;
		else ani = MS_ANI_LV1_LEFT;
		break;
	case MS_STATE_LV2:
		if (nx_simon>0) ani = MS_ANI_LV2_RIGHT;
		else ani = MS_ANI_LV2_LEFT;
		break;
	case MS_STATE_LV3:
		if (nx_simon>0) ani = MS_ANI_LV3_RIGHT;
		else ani = MS_ANI_LV3_LEFT;
		break;

	}
	animations[ani]->Render(x - cameraPosX, y - cameraPosY);
	RenderBoundingBox(cameraPosX, cameraPosY);
}

void CMorningStar::Render(float & cameraPosX, float & cameraPosY)
{
}

void CMorningStar::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (is_last_frame) {
		int top_modify = 7;
		int left_modify = 30;
		if (nx < 0) {
			l = x;
		}
		else {
			l = x + left_modify;

		}
		t = y + top_modify;
		r = l + width;
		b = t + height;

		
	}
	else {
		l = 0;
		r = 0;
		b = 0;
		t = 0;
	}
}

void CMorningStar::isColidingObjects(
	vector<LPGAMEOBJECT> *coObjects,
	vector<LPGAMEOBJECT> &colidingObjects) {

	float otherL;
	float otherT;
	float otherB;
	float otherR;

	float objectL;
	float objectT;
	float objectB;
	float objectR;
	GetBoundingBox(objectL, objectT, objectR, objectB);

	for (int i = 0; i < coObjects->size(); i++) {
		coObjects->at(i)->GetBoundingBox(otherL, otherT, otherR, otherB);

		if (otherL <= objectR &&
			otherR >= objectL &&
			otherT <= objectB &&
			otherB >= objectT) {
			colidingObjects.push_back(coObjects->at(i));
		}
		

	}

}

void CMorningStar::upgrade()
{
	if (state == MS_STATE_LV1)
		state = MS_STATE_LV2;
	else
		if (state == MS_STATE_LV2) {
			state = MS_STATE_LV3;
			xModifyR = 11;
			xModifyL = 42;
			width = 42;
		}
}
