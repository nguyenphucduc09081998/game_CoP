#include "Bat.h"



CBat::CBat(float x, float y, int nx) : CEnemy(x, y, nx)
{
	this->AddAnimation(6020);
	this->AddAnimation(6021);
	this->AddAnimation(6022);
	this->AddAnimation(502);
	damageDeal = 2;
	this->height = 16;
	this->width = 16;
	this->vy = BAT_MOVE_UP_SPEED;

	action = 0;
	if (nx > 0) {
		this->vx = BAT_MOVE_SPEED;
		canUpdate = false;
	}
	else {
		this->vx = -BAT_MOVE_SPEED;
		canUpdate = true;
	}
	enemyType = BAT_ID;
}

void CBat::Update(DWORD dt, vector<LPGAMEOBJECT>* CreepColiObjects, float SimonPosx, bool stopEnemy)
{
	if (state != STATE_REMOVED) {
		this->stopEnemy = stopEnemy;

		if (state == STATE_BEING_REMOVE) {
			if (GetTickCount() - destroy_start > DESTROY_TIME) {
				state = STATE_REMOVED;
				_x = x;
				return;
			}
		}
		else {
			action += dy;
			if (action > 16) {
				vy = BAT_MOVE_UP_SPEED;
				action = 0;
			}
			else {
				if (action < -16) {
					vy = BAT_MOVE_DOWN_SPEED;
					action = 0;
				}
			}

			if (!stopEnemy) {
				CGameObject::Update(dt);
			}
			else {
				dx = 0;
				dy = 0;
			}
			vector<LPCOLLISIONEVENT> coEvents;
			vector<LPCOLLISIONEVENT> coEventsResult;

			coEvents.clear();
			CalcPotentialCollisions(CreepColiObjects, coEvents);
			if (coEvents.size() == 0)
			{
				x += dx;
				y += dy;
			}
			else
			{
				float min_tx, min_ty, nx = 0, ny;
				FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
				x += dx;
				y += dy;
			}
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}
	}
}

void CBat::Render(float & cameraPosX, float & cameraPosY)
{
	if (state == STATE_BEING_REMOVE || state == STATE_REMOVED) {
		animations[3]->Render(x - cameraPosX, y - cameraPosY);
	}
	else {
		if (nx > 0) {
			if (!stopEnemy) {
				animations[2]->Render(x - cameraPosX, y - cameraPosY);
			}
			else {
				animations[2]->RenderCurrentFrame(x - cameraPosX, y - cameraPosY);
			}
		}
		else {
			if (!stopEnemy) {
				animations[1]->Render(x - cameraPosX, y - cameraPosY);
			}
			else {
				animations[1]->RenderCurrentFrame(x - cameraPosX, y - cameraPosY);
			}
		}
	}
}

CBat::~CBat()
{
}
