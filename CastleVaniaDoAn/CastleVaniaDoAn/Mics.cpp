#include "Mics.h"

void CMics::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
	if (state == STATE_BEING_REMOVE) {
		if (GetTickCount() - destroy_start > DESTROY_TIME) {
			_x = x;
			this->x = -1000; // remove from sence
			state = STATE_REMOVED;
		}
	}
}

CMics::CMics(int micsType, int itemDrop, float x, float y)
{
	this->x = x;
	this->y = y;
	type = GO_TYPE_MICS;
	this->itemDrop = itemDrop;
	if (micsType == FIRE_STATUE_ID) {
		width = 16;
		height = 32;
		AddAnimation(500);
		AddAnimation(502);
	}
	else { // candle
		width = 8;
		height = 16;
		AddAnimation(501);
		AddAnimation(502);
	}
}

void CMics::Render(float &cameraPosX, float &cameraPosY)
{
	if (state == STATE_BEING_REMOVE) {
		animations[1]->Render(x - cameraPosX, y - cameraPosY);
		
	}
	else {
		animations[0]->Render(x - cameraPosX, y - cameraPosY);
	}
	RenderBoundingBox(cameraPosX, cameraPosY);
}

void CMics::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
void CMics::SetState(int state)
{
	switch (state) {
	case STATE_BEING_REMOVE:
		this->state = STATE_BEING_REMOVE;
		destroy_start = GetTickCount();
		break;
	}
}

LPCITEM CMics::DropItem()
{
	CItems *item;
	item = new CItems(itemDrop, _x,y);
	this->state = STATE_NOUPDATE;
	return item;
}

