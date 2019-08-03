#include "Enemy.h"



CEnemy::CEnemy(float x, float y, int nx)
{
	this->x = x;
	this->y = y;
	this->nx = nx;
	this->type = 6;
	this->itemDrop = 0;
}
void CEnemy::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
LPCITEM CEnemy::DropItem()
{
	CItems *item;
	int change = rand() % 100;
	if (change > 70) {
		item = new CItems(itemDrop, _x, y);
	}
	else {
		return NULL;
	}
	this->state = STATE_NOUPDATE;
	return item;
}
void CEnemy::SetState(int state)
{
	switch (state) {
	case STATE_BEING_REMOVE:
		this->damageDeal = 0;
		this->state = STATE_BEING_REMOVE;
		destroy_start = GetTickCount();
		vx = 0;
		vy = 0;
		break;
	}
}
void CEnemy::attack()
{
	if (state != STATE_BEING_REMOVE && state != STATE_REMOVED) {
		attackStart = GetTickCount();
		canAttack = true;
		state = FISH_STATE_ATTACK;
		vx = 0;
	}
}
CEnemy::~CEnemy()
{
}

