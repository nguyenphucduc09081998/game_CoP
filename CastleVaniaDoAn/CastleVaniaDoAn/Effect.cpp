#include "Effect.h"



CEffect::CEffect(int effecttype, float x, float y, float vx, float vy, int flag)
{
	if (effecttype == 1) 
		this->AddAnimation(1300);
	else
		this->AddAnimation(1301);
	this->type = GO_TYPE_EFFECT;
	this->x = x;
	this->y = y;
	this->vx = vx;
	this->vy = vy;
}

CEffect::CEffect(float x, float y, float vx, float vy, int nx )
{
	type = GO_TYPE_EFFECT_BULLET;
	this->nx = nx;
	this->x = x;
	this->y = y;
	this->vx = (nx>0) ? abs(vx) : -abs(vx);
	this->vy = vy;
	this->AddAnimation(1303);
	this->AddAnimation(1302);
	this->AddAnimation(502);
}


CEffect::~CEffect()
{
}

void CEffect::Render(float & cameraPosX, float & cameraPosY)
{
	if (type == GO_TYPE_EFFECT_BULLET) {
		if (state == STATE_BEING_REMOVE || state == STATE_REMOVED) {
			animations[2]->Render(x - cameraPosX, y - cameraPosY);
		}
		else {
			if (nx > 0) {
				animations[0]->Render(x - cameraPosX, y - cameraPosY);
			}
			else
				animations[1]->Render(x - cameraPosX, y - cameraPosY);
		}
	}
	else {
		animations[0]->Render(x - cameraPosX, y - cameraPosY);
	}
	RenderBoundingBox(x - cameraPosX, y - cameraPosY);
}

void CEffect::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + 6;
	b = y + 6;
}

void CEffect::Update(DWORD dt)
{
	if (state == STATE_BEING_REMOVE) {
		if (GetTickCount() > destroy_start + DESTROY_TIME) {
			state = STATE_REMOVED;
		}

	}
	CGameObject::Update(dt);
	x += dx;
	y += dy;
}
void CEffect::SetState(int state) {
	this->state = state;
	if (state == STATE_BEING_REMOVE) {
		vx = 0;
		vy = 0;
		destroy_start = GetTickCount();
	}
}