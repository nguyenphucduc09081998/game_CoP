#include "SubWeapon.h"



void CSubWeapon::Update(DWORD dt, vector<LPGAMEOBJECT> *WeaponColiObjects)
{
	vx = nx * abs(vx);
	if (state == WEAPON_STATE_FIRE_BURN) {
		if (GetTickCount() > timeStart + WEAPON_FIRE_LASTING) {
			this->Remove();
		}

	}
	//if (weaponType != SWORD_ID) {
		vy += BASE_GRAVITY * dt;

	//}
	if (state == WEAPON_STATE_FIRE_BURN) {
		vx = 0;
		vy = 0;

	}
	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(WeaponColiObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CGameObject *>(e->obj))
			{
				CGameObject *object = dynamic_cast<CGameObject *>(e->obj);
				switch (object->type) {
				case GO_TYPE_MICS:
					if (weaponType == 4) {
						this->SetState(STATE_REMOVED);
					}
					object->SetState(STATE_BEING_REMOVE);
					break;
				case GO_TYPE_ENEMY:
					if (weaponType == 4) {
						this->SetState(STATE_REMOVED);
					}
					object->SetState(STATE_BEING_REMOVE);
					break;
				case GO_TYPE_BRICK:
					if (weaponType == FIRE_BOTTLE_ID) {
						this->SetState(WEAPON_STATE_FIRE_BURN);
						vx = 0;
						vy = 0;
						dx = 0;
						dy = 0;
						timeStart = GetTickCount();
					}
					break;
				case GO_TYPE_BOSS:
					CBoss * boss = dynamic_cast<CBoss*>(e->obj);
					switch (weaponType) {
					case FIRE_BOTTLE_ID:
						boss->TakeDamage(1);
						break;
					case SWORD_ID:
						boss->TakeDamage(1);
						break;
					case AXE_ID:
						boss->TakeDamage(2);
						break;
					}
					


				}
			}

			
		}
		x += dx;
		y += dy;
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	
}

CSubWeapon::CSubWeapon(int weaponType, int n)
{
	this->type = 11;
	this->weaponType = weaponType;
	this->nx = n;
	switch (weaponType) {
	case SWORD_ID:
		this->AddAnimation(7013);
		this->AddAnimation(7017);
		width = 16;
		height = 9;
		vx = 0.12;
		vy = -0.5;
		break;
	case 5:
		break;
	case AXE_ID:
		vy = -0.5;
		this->AddAnimation(7015);
		this->AddAnimation(7020);
		width = 15;
		height = 15;
		vx = 0.12;
		break;
	case FIRE_BOTTLE_ID:
		vy = -0.3;
		this->AddAnimation(7014);
		this->AddAnimation(7018);
		this->AddAnimation(7019);		
		width = 15;
		height = 13;
		vx = 0.15;
		break;
	}
}


CSubWeapon::~CSubWeapon()
{
}


void CSubWeapon::Render(float & cameraPosX, float & cameraPosY)
{
	if (state == WEAPON_STATE_FIRE_BURN && weaponType == 8) {
		animations[2]->Render(x - cameraPosX, y - cameraPosY);

	}else
	if (nx > 0) {
		animations[1]->Render(x - cameraPosX, y - cameraPosY);
	}
	else {
		animations[0]->Render(x - cameraPosX, y - cameraPosY);
	}
	RenderBoundingBox(cameraPosX, cameraPosY);
}

void CSubWeapon::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;

}

void CSubWeapon::Remove()
{
	this->x = -1000; // remove from sence
	state = STATE_REMOVED;
}
