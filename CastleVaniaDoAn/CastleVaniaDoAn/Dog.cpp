#include "Dog.h"



CDog::CDog(float x, float y, int nx, float distanceBeforeJump):CEnemy(x, y, nx)
{
	this->AddAnimation(6012);
	this->AddAnimation(6013);
	this->AddAnimation(6010);
	this->AddAnimation(6011);
	this->AddAnimation(6014);
	this->AddAnimation(6015);
	this->AddAnimation(502);
	damageDeal = 2;
	this->height = 16;
	this->width = 32;
	this->vx = 0;
	this->vy = 0;
	state = DOG_STATE_IDLE;
	enemyType = DOG_ID;
	action = distanceBeforeJump;
}

void CDog::Update(DWORD dt, vector<LPGAMEOBJECT>* CreepColiObjects, float SimonPosx, bool stopEnemy)
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
			if (state == DOG_STATE_IDLE) {
				if (x - SimonPosx < 100) {
					state = DOG_STATE_MOVE;
					if (nx > 0) {
						vx = DOG_SPEED;
					}
					else {
						vx = -DOG_SPEED;
					}
				}
			}
			else {
				if (state == DOG_STATE_MOVE) {
					action -= abs(dx);
				}
				if (action < 0) {
					action = 0;
					state = DOG_STATE_JUMP;
					vy = -0.12;


				}
				if (state == DOG_STATE_MOVE_AFTER_JUMP) {
					vy = 0.075;
				}
				vy += 0.0003 * dt;
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
				for (UINT i = 0; i < coEventsResult.size(); i++)
				{
					LPCOLLISIONEVENT e = coEventsResult[i];
					if (e->obj->type == 2 && state == DOG_STATE_JUMP && nx == 0) {
						state = DOG_STATE_MOVE_AFTER_JUMP;
						this->nx = -this->nx;
						vx = (this->nx > 0) ? DOG_SPEED : -DOG_SPEED;
					}
					if (nx != 0) {
						x += dx;
					}
				}
				x += min_tx * dx + nx * 0.1f;		
				y += min_ty * dy + ny * 0.1f;
				if (ny != 0) vy = 0;
			}
			for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		}
	}
}

void CDog::Render(float & cameraPosX, float & cameraPosY)
{
	if (state == STATE_BEING_REMOVE || state == STATE_REMOVED) {
		animations[6]->Render(x - cameraPosX, y - cameraPosY);
	}
	else {
		if (nx > 0) {
			if (state == DOG_STATE_IDLE) {
				if (!stopEnemy) {
					animations[1]->Render(x - cameraPosX, y - cameraPosY);
				}
				else {
					animations[1]->RenderCurrentFrame(x - cameraPosX, y - cameraPosY);
				}
			}
			else {
				if (state == DOG_STATE_JUMP) {

					animations[5]->Render(x - cameraPosX, y - cameraPosY);
				}
				else {
					if (!stopEnemy) {
						animations[0]->Render(x - cameraPosX, y - cameraPosY);
					}
					else {
						animations[0]->RenderCurrentFrame(x - cameraPosX, y - cameraPosY);
					}
				}
			}
		}
		else {
			if (state == DOG_STATE_IDLE) {
				if (!stopEnemy) {
					animations[2]->Render(x - cameraPosX, y - cameraPosY);
				}
				else {
					animations[2]->RenderCurrentFrame(x - cameraPosX, y - cameraPosY);
				}
			}
			else {
				if (state == DOG_STATE_JUMP) {

					animations[4]->Render(x - cameraPosX, y - cameraPosY);
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
			
		}
	}



}


CDog::~CDog()
{
}
