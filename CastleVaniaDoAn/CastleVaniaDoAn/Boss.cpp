#include "Boss.h"

#define BOSS_STATE_AFTER_COLI 5

CBoss::CBoss(float x, float y)
{
	AddAnimation(6040);
	AddAnimation(6041);
	AddAnimation(6042);
	this->x = x;
	this->y = y;
	width = 48;
	height = 23;
	state = BOSS_STATE_SLEEP;
	vx = 0;
	vy = 0;
	type = GO_TYPE_BOSS;
	totalMovingDistance = 0;
	bossHeal = 16;
}


CBoss::~CBoss()
{
}

void CBoss::Render(float & cameraPosX, float & cameraPosY)
{
	if (state == BOSS_STATE_SLEEP) {
		animations[0]->Render(x - cameraPosX, y - cameraPosY);
	}
	else {
		if (state == STATE_BEING_REMOVE||state==STATE_REMOVED) {
			animations[2]->Render(x - cameraPosX, y - cameraPosY);

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
	RenderBoundingBox(cameraPosX, cameraPosY);
}

void CBoss::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

void CBoss::Update(DWORD dt, CSimon * Simon, bool stopEnemy)
{
	this->stopEnemy = stopEnemy;
	(vx > 0) ? nx = 1 : nx = -1;
	if (x < 514 || x > 748) {
		vx = -vx;
		if (y > 120) {
			vy = -0.06;
		}
		else {
			vy = +0.06;
		}
		state = BOSS_STATE_AFTER_COLI;
		actionTime = GetTickCount()- 1000;
	}
	if (y > 210 && state != BOSS_STATE_SLEEP) {
		vy = -0.1;
		state = BOSS_STATE_AFTER_COLI;
		actionTime = GetTickCount() - 1000;
	}

	if (state == BOSS_STATE_SLEEP) {
		if (Simon->x > 674) {
			Simon->stopCamera = true;
			actionTime = GetTickCount();
			state = BOSS_STATE_MOVE;
			vx = -BOSS_MOVE_SPEED;
			vy = BOSS_MOVE_SPEED;
		}
	}
	if (state == BOSS_STATE_IDLE) {
		vx = 0;
		vy = 0;
	}
	if (state == BOSS_STATE_ATTACK) {
		if (GetTickCount() > actionTime + BOSS_ACTION_COOLDOWN_TIME/2 && !flipYFlag) {
			vy = -abs(vy);
			flipYFlag = true;
		}
		else {
			flipYFlag = false;

		}
		if (y > 120) {

			vy -= 0.00002*dt;
		
		
		}
	}


	//change state for boss
	if (GetTickCount() > actionTime + BOSS_ACTION_COOLDOWN_TIME) {
		actionTime = GetTickCount();
		if (state == BOSS_STATE_MOVE) {
			vx = 0;
			vy = 0;
			state = BOSS_STATE_IDLE;
		}
		else {
			if (state == BOSS_STATE_IDLE) {
				tmpX = x;
				tmpSimonX = Simon->x;
				tmpSimonY = Simon->y;
				if (Simon->y < y-30) {
					canAttack = true;
					state = BOSS_STATE_RANGE_ATTACK;
					
				}
				else {
					state = BOSS_STATE_ATTACK;
					flipYFlag = false;
					if (abs(tmpX - tmpSimonX) < 30) {
						vy = BOSS_ATTACK_MOVE_SPEED;
						if (tmpX < tmpSimonX) {
							vx = BOSS_ATTACK_MOVE_SPEED;
						}
						else {
							vx = -BOSS_ATTACK_MOVE_SPEED;
						}
					}
					else {
						if (abs(tmpX - tmpSimonX) < 60) {
							vy = BOSS_ATTACK_MOVE_SPEED;
							if (tmpX < tmpSimonX) {
								vx = BOSS_ATTACK_MOVE_SPEED;
							}
							else {
								vx = -BOSS_ATTACK_MOVE_SPEED;
							}
						}
						else {
							vy = BOSS_ATTACK_MOVE_SPEED;
							if (tmpX < tmpSimonX) {
								vx = BOSS_ATTACK_MOVE_SPEED;
							}
							else {
								vx = -BOSS_ATTACK_MOVE_SPEED;
							}
						}
					}
				}
			}
			else {
				if (state == BOSS_STATE_ATTACK) {
					actionTime = GetTickCount() - 1000;
					state = BOSS_STATE_MOVE;
					if (x < 630) {
						vx = BOSS_ATTACK_MOVE_SPEED;

					}
					else {
						vx = -BOSS_ATTACK_MOVE_SPEED;
					}

					if (y > 120) {
						vy = -0.06;
					}
					else {
						vy = 0.06;

					}
				}
				else {
					if (state == BOSS_STATE_AFTER_COLI) {
						vx = 0;
						vy = 0;
						state = BOSS_STATE_IDLE;
					}
					else {
						if (state == BOSS_STATE_RANGE_ATTACK) {
							vx = 0;
							vy = 0;
							state = BOSS_STATE_IDLE;
						}
						else {
							if (state == STATE_BEING_REMOVE) {
								state = STATE_REMOVED;
							}
						}
					}
				}
			}
		}
	}
	if (bossHeal == 0 && state != STATE_BEING_REMOVE && state != STATE_REMOVED) {
		tmpX = x;
		actionTime = GetTickCount() - 1000;
		state = STATE_BEING_REMOVE;
		vx = 0;
		vy = 0;
	}

	if (!stopEnemy) {
		CGameObject::Update(dt);
		totalMovingDistance += dx;
		totalMovingDistance += dy;
		x += dx;
		y += dy;
	}




}

void CBoss::TakeDamage(int damage)
{
	if (GetTickCount() > takeDamageCoolDown + 300) {
		this->bossHeal -= damage;
		if (bossHeal < 0) {
			bossHeal = 0;
		}
	}
	takeDamageCoolDown = GetTickCount();
}

	
