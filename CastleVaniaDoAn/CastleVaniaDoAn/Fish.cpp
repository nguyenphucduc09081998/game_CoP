#include "Fish.h"


CFish::CFish(float x, float y, int nx) : CEnemy(x, y, nx)
{
	canUpdate = false;
	this->AddAnimation(6030);
	this->AddAnimation(6031);
	this->AddAnimation(6032);
	this->AddAnimation(6033);
	this->AddAnimation(6034);
	this->AddAnimation(6035);
	this->AddAnimation(502);
	damageDeal = 2;
	this->height = 32;
	this->width = 16;
	this->vy = -0.3;
	action = 0;
	state = FISH_STATE_JUMP;
	vx = 0;
	enemyType = FISH_ID;
}

void CFish::Update(DWORD dt, vector<LPGAMEOBJECT>* CreepColiObjects, float SimonPosx, bool stopEnemy)
{
	if (state != STATE_REMOVED) {
		this->stopEnemy = stopEnemy;

		if (state == STATE_BEING_REMOVE) {
			if (GetTickCount() - destroy_start > DESTROY_TIME) {
				state = STATE_REMOVED;
				_x = x;
				//this->x = -100; // remove from sence
				return;
			}
		}
		else {
			if (state == FISH_STATE_JUMP && y < 60) {

				vy += BASE_GRAVITY * dt;
				vx = (nx > 0) ? FISH_MOVE_SPEED : -FISH_MOVE_SPEED;
				state = FISH_STATE_MOVE;
				attackStart = GetTickCount();
			}
			if (state != FISH_STATE_JUMP && state != STATE_BEING_REMOVE) {
				vy += BASE_GRAVITY * dt;
				if (attackStart + FISH_ATTACK_DURATION < GetTickCount()) {
					vx = (nx > 0) ? FISH_MOVE_SPEED : -FISH_MOVE_SPEED;
					state = FISH_STATE_MOVE;
				}
				if (attackStart + FISH_ATTACK_COOLDOWN < GetTickCount()) {
					attack();
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
				if (state == FISH_STATE_JUMP) {
					x += dx;
					y += dy;
				}
				else {
					x += min_tx * dx + nx * 0.1f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
					y += min_ty * dy + ny * 0.1f;
					if (ny != 0) vy = 0;

				}
			}
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}
	}
}

void CFish::Render(float & cameraPosX, float & cameraPosY)
{
	if (state == STATE_BEING_REMOVE || state == STATE_REMOVED) {
		animations[6]->Render(x - cameraPosX, y - cameraPosY);
	}
	else {
		if (state == FISH_STATE_JUMP) {
			if (nx < 0) {
				animations[0]->Render(x - cameraPosX, y - cameraPosY);
			}
			else {
				animations[1]->Render(x - cameraPosX, y - cameraPosY);
			}
		}
		else {
			if (state == FISH_STATE_MOVE) {
				if (nx < 0) {
					if (!stopEnemy) {
						animations[2]->Render(x - cameraPosX, y - cameraPosY);
					}
					else {
						animations[2]->RenderCurrentFrame(x - cameraPosX, y - cameraPosY);
					}
				}
				else {
					if (!stopEnemy) {
						animations[3]->Render(x - cameraPosX, y - cameraPosY);
					}
					else {
						animations[3]->RenderCurrentFrame(x - cameraPosX, y - cameraPosY);
					}
				}
			}
			else {
				if (state == FISH_STATE_ATTACK) {
					if (nx < 0) {
						if (!stopEnemy) {
							animations[4]->Render(x - cameraPosX, y - cameraPosY);
						}
						else {
							animations[4]->RenderCurrentFrame(x - cameraPosX, y - cameraPosY);
						}
					}
					else {
						if (!stopEnemy) {
							animations[5]->Render(x - cameraPosX, y - cameraPosY);
						}
						else {
							animations[5]->RenderCurrentFrame(x - cameraPosX, y - cameraPosY);
						}
					}
				}
			}
		}
	}
}

CFish::~CFish()
{
}
