#include <algorithm>
#include "Simon.h"
#include "Game.h"

void CSimon::Update(DWORD dt, float &cameraPosX, float &cameraPosY, int stage_width, CMorningStar* morningStar, vector<LPGAMEOBJECT> *coObjects, vector<LPCSTAIR> Stairs, int& score)
{
	//reset untouchable
	if (untouchable != 0) {
		if (untouchable == 3) {
			if (GetTickCount() > untouchable_start + INVI_DURATION) {
				untouchable = 0;
			}
		}
		else {
			if (untouchable == 1) {
				vx = 0.1;
			}
			else
				if (untouchable == -1) {
					vx = -0.1;
				}
			if (GetTickCount() > untouchable_start + SIMON_TAKINGDAME_TIME) {
				untouchable = 2;
			}
			if (untouchable_start + SIMON_UNTOUCHABLE_TIME < GetTickCount()) {
				untouchable = 0;
			}
		}
	}

	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down

	//Stair handle
	for (int i = 0; i < Stairs.size(); i++) {
		if (Stairs[i]->nx > 0) {
			if (x >= Stairs[i]->x - 8 && x <= Stairs[i]->x + Stairs[i]->width + 8 &&  y <= Stairs[i]->y && y< Stairs[i]->y && y > Stairs[i]->y - Stairs[i]->width - 38 ) {
				currentStair = Stairs[i];
			}
		}
		else {
			if (x <= Stairs[i]->x + 8 && x >= Stairs[i]->x - Stairs[i]->width - 8 && y <= Stairs[i]->y && y< Stairs[i]->y && y > Stairs[i]->y - Stairs[i]->width - 38) {
				currentStair = Stairs[i];
			}
		}
	}
	if (currentStair) {

		if (x >= currentStair->x - 16 && x <= currentStair->x + 16 && currentStair->y-y >5 && currentStair->y - y < SIMON_STAND_BBOX_HEIGHT+1) {
			nearUpStair = true;
		}
		else {
			nearUpStair = false;
		}
		if (currentStair->nx > 0 && x >= currentStair->x - 16 + currentStair->width && x <= currentStair->x + 16 + currentStair->width
			|| currentStair->nx < 0 && x >= currentStair->x - 16 - currentStair->width && x <= currentStair->x + 16 - currentStair->width) {
			nearDownStair = true;
		}
		else {
			nearDownStair = false;
		}
		if (action == SIMON_ACTION_GO_ON_STAIR) {
			GoOnStair();
		}

		if (onStair) {
			if (currentStair->nx > 0) {
				if (x >= currentStair->x -8 && x <= currentStair->x + currentStair->width-3) {
					if (state == SIMON_STATE_UP_STAIR_RIGHT) {
						posXforStair += SIMON_WALKING_SPEED * dt;
						posYforStair -= SIMON_WALKING_SPEED * dt;

						if (posXforStair >= 1) {
							posXforStair = 0;
							x += 1;

							tempXforStair++;
							if (tempXforStair == 8) {
								tempXforStair = 0;
							}
						}
						if (posYforStair <= -1) {
							posYforStair = 0;
							y -= 1;

							tempYforStair++;
							if (tempYforStair == 8) {
								tempYforStair = 0;
							}
						}
					}
					if (state == SIMON_STATE_DOWN_STAIR_LEFT) {
						posXforStair += SIMON_WALKING_SPEED * dt;
						posYforStair -= SIMON_WALKING_SPEED * dt;

						if (posXforStair >= 1) {
							posXforStair = 0;
							x -= 1;
							tempXforStair++;
							if (tempXforStair == 8) {
								tempXforStair = 0;
							}
						}
						if (posYforStair <= -1) {
							posYforStair = 0;
							y += 1;

							tempYforStair++;
							if (tempYforStair == 8) {
								tempYforStair = 0;
							}

							if (y + SIMON_STAND_BBOX_HEIGHT > currentStair->y) {
								y = currentStair->y - 0.04 - SIMON_STAND_BBOX_HEIGHT;
							}
						}
					}
				}
				else {
					onStair = false;
				}

			}
			else {
				if (x <= currentStair->x + 8 && x >= currentStair->x - currentStair->width + 3) {
					if (state == SIMON_STATE_UP_STAIR_LEFT) {
						posXforStair += SIMON_WALKING_SPEED * dt;
						posYforStair -= SIMON_WALKING_SPEED * dt;

						if (posXforStair >= 1) {
							posXforStair = 0;
							x -= 1;

							tempXforStair++;
							if (tempXforStair == 8) {
								tempXforStair = 0;
							}
						}
						if (posYforStair <= -1) {
							posYforStair = 0;
							y -= 1;

							tempYforStair++;
							if (tempYforStair == 8) {
								tempYforStair = 0;
							}
						}
					}
					if (state == SIMON_STATE_DOWN_STAIR_RIGHT) {
						posXforStair += SIMON_WALKING_SPEED * dt;
						posYforStair -= SIMON_WALKING_SPEED * dt;

						if (posXforStair >= 1) {
							posXforStair = 0;
							x += 1;

							tempXforStair++;
							if (tempXforStair == 8) {
								tempXforStair = 0;
							}
						}
						if (posYforStair <= -1) {
							posYforStair = 0;
							y += 1;

							tempYforStair++;
							if (tempYforStair == 8) {
								tempYforStair = 0;
							}

							if (y + SIMON_STAND_BBOX_HEIGHT > currentStair->y) {
								y = currentStair->y - 0.04 - SIMON_STAND_BBOX_HEIGHT;
							}
						}
					}
				}
				else {
					onStair = false;
				}

			}

			if (state == SIMON_STATE_UP_STAIR_IDLE) {
				if (nx > 0) {
					if (tempXforStair != 0) {
						posXforStair += SIMON_WALKING_SPEED * dt;
						if (posXforStair >= 1) {
							posXforStair = 0;
							x += 1;

							tempXforStair++;
							if (tempXforStair == 8) {
								tempXforStair = 0;
							}
						}
					}
					if (tempYforStair != 0) {
						posYforStair -= SIMON_WALKING_SPEED * dt;
						if (posYforStair <= -1) {
							posYforStair = 0;
							y -= 1;

							tempYforStair++;
							if (tempYforStair == 8) {
								tempYforStair = 0;
							}
						}
						if (y + SIMON_STAND_BBOX_HEIGHT > currentStair->y) {
							y = currentStair->y - 0.04 - SIMON_STAND_BBOX_HEIGHT;
						}
					}
				}
				else {
					if (tempXforStair != 0) {
						posXforStair -= SIMON_WALKING_SPEED * dt;
						if (posXforStair <= -1) {
							posXforStair = 0;
							x -= 1;

							tempXforStair++;
							if (tempXforStair == 8) {
								tempXforStair = 0;
							}
						}
					}
					if (tempYforStair != 0) {
						posYforStair -= SIMON_WALKING_SPEED * dt;
						if (posYforStair <= -1) {
							posYforStair = 0;
							y -= 1;

							tempYforStair++;
							if (tempYforStair == 8) {
								tempYforStair = 0;
							}
						}
						if (y + SIMON_STAND_BBOX_HEIGHT > currentStair->y) {
							y = currentStair->y - 0.04 - SIMON_STAND_BBOX_HEIGHT;
						}
					}
				}
			}
			if (state == SIMON_STATE_DOWN_STAIR_IDLE) {
				if (nx > 0) {
					if (tempXforStair != 0) {
						posXforStair -= SIMON_WALKING_SPEED * dt;
						if (posXforStair <= -1) {
							posXforStair = 0;
							x += 1;

							tempXforStair++;
							if (tempXforStair == 8) {
								tempXforStair = 0;
							}
						}
					}
					if (tempYforStair != 0) {
						posYforStair += SIMON_WALKING_SPEED * dt;
						if (posYforStair >= 1) {
							posYforStair = 0;
							y += 1;

							tempYforStair++;
							if (tempYforStair == 8) {
								tempYforStair = 0;
							}
						}
						if (y + SIMON_STAND_BBOX_HEIGHT > currentStair->y) {
							y = currentStair->y - 0.04 - SIMON_STAND_BBOX_HEIGHT;
						}
					}
				}
				else {
					if (tempXforStair != 0) {
						posXforStair -= SIMON_WALKING_SPEED * dt;
						if (posXforStair <= -1) {
							posXforStair = 0;
							x -= 1;

							tempXforStair++;
							if (tempXforStair == 8) {
								tempXforStair = 0;
							}
						}
					}
					if (tempYforStair != 0) {
						posYforStair += SIMON_WALKING_SPEED * dt;
						if (posYforStair >= 1) {
							posYforStair = 0;
							y += 1;
							tempYforStair++;
							if (tempYforStair == 8) {
								tempYforStair = 0;
							}
						}
						if (y + SIMON_STAND_BBOX_HEIGHT > currentStair->y) {
							y = currentStair->y - 0.04 - SIMON_STAND_BBOX_HEIGHT;
						}
					}
				}
			}
		}
	}
	///

	if (!onStair) {
		vy += SIMON_GRAVITY * dt;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	//if (state != SIMON_STATE_DIE)
	CalcPotentialCollisions(coObjects, coEvents);

	if (attacking) {
		dx = 0;
		if (GetTickCount() - attack_start > SIMON_TIME_TO_DISPLAY_LAST_FRAME) {
			isLastFrame = true;
		}
		else {
			isLastFrame = false;
		
		}
		if (GetTickCount() - attack_start > SIMON_ATTACK_DURATION) {
			attack_start = 0;
			attacking = 0;
			created_subweapon = 0;
		}

	}
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.2f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.2f;
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		// Collision logic with Item
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CItems *>(e->obj))
			{
				CItems *item = dynamic_cast<CItems *>(e->obj);
				switch (item->item_type) {
				case SMALL_HEART_ID:
					this->heart += 1;
					break;
				case BIG_HEART_ID:
					this->heart += 5;
					break;
				case BAG_1_ID:
					score += 100;
					break;
				case BAG_2_ID:
					score += 700;
					break;
				case BAG_3_ID:
					score += 400;
					break;
				case MORNING_STAR_ID:
					morningStar->upgrade();
					break;
				case SWORD_ID:
					this->subWeapon = SWORD_ID;
					break;
				case CLOCK_ID:
					this->subWeapon = CLOCK_ID;
					break;
				case NECKLET_ID:
					destroyEnemy = true;
					break;
				case AXE_ID:
					this->subWeapon = AXE_ID;
					break;
				case FIRE_BOTTLE_ID:
					this->subWeapon = FIRE_BOTTLE_ID;
					break;
				case CHICKEN_ID:
					this->player_heal += 5;
					if (player_heal > 16) {
						player_heal = 16;
					}
					break;
				case INVI_POTION_ID:
					untouchable = 3;
					untouchable_start = GetTickCount();
					break;
					
				}
				item->Remove();
				
			}
			if (e->obj->type == GO_TYPE_BOSS) {
				x += dx;
				
				if (untouchable == 0) {
						this->player_heal -= 2;
						takeDame(e->obj->nx);
					if (this->player_heal < 0) {
						this->player_heal = 0;
					}
					
				}
			}
			if (dynamic_cast<CEnemy*>(e->obj)) {
				CEnemy *enemy = dynamic_cast<CEnemy *>(e->obj);
				if (untouchable == 0 && enemy->canUpdate) {
					
					
					
					if (enemy->damageDeal) {
						this->player_heal -= enemy->damageDeal;
						takeDame(enemy->nx);
					}
					if (this->player_heal < 0) {
						this->player_heal = 0;
					}
					if (enemy->enemyType == 3) {
						enemy->SetState(STATE_BEING_REMOVE);
					}
				}
				else {
					x += dx;
				}
			}
			if (dynamic_cast<CEffect*>(e->obj)) {
				if (untouchable == 0) {
					CEffect *bullet = dynamic_cast<CEffect *>(e->obj);
					if (bullet->state != STATE_BEING_REMOVE && bullet->state != STATE_REMOVED) {
						this->player_heal -= 1;
						takeDame(bullet->nx);
						if (this->player_heal < 0) {
							this->player_heal = 0;
						}
					}
				}
				else {
					x += dx;
				}
			}

		}
		// clean up collision events
		
		//move camera
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	if ((x > cameraPosX + 240 / 2 && cameraPosX + SCREEN_WIDTH < stage_width || x < cameraPosX + 240 / 2 && cameraPosX > 0)&& !stopCamera) { // 240 is screen width
		cameraPosX = x - 240 / 2;
	}
	if (x < 0) {
		x = 0;
	}
	if (x < cameraPosX) {
		x = cameraPosX;
	}
}

void CSimon::Render(float &cameraPosX, float &cameraPosY)
{
	int ani;

	if (untouchable == -1) {
		ani = SIMON_ANI_TAKING_DAMAGE_LEFT;
	}
	else 
	if (untouchable == 1) {
		ani = SIMON_ANI_TAKING_DAMAGE_RIGHT;
	}
	else
	if (attacking) {

		if (state == SIMON_STATE_QUY) {
			if (nx>0) ani = SIMON_ANI_QUY_RIGHT;
			else ani = SIMON_ANI_QUY_LEFT;
		}
		else
			if (nx>0) ani = SIMON_ANI_ATTACK_RIGHT;
			else ani = SIMON_ANI_ATTACK_LEFT;
	}
	else
	if (action == SIMON_ACTION_GO_ON_STAIR) {
		if(nx>0)
			ani = SIMON_ANI_WALKING_RIGHT;
		else
			ani = SIMON_ANI_WALKING_LEFT;
	}
	else
	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else
	if (state == SIMON_STATE_UP_STAIR_IDLE) {
		if (nx>0) ani = SIMON_ANI_IDLE_RIGHT_UP_STAIR;
		else ani = SIMON_ANI_IDLE_LEFT_UP_STAIR;
	}else
	if (state == SIMON_STATE_DOWN_STAIR_IDLE) {
		if (nx>0) ani = SIMON_ANI_IDLE_RIGHT_DOWN_STAIR;
		else ani = SIMON_ANI_IDLE_LEFT_DOWN_STAIR;
	}
	else
	if (state == SIMON_STATE_UP_STAIR_RIGHT)
		ani = SIMON_ANI_RIGHT_UP_STAIR;
	else
	if (state == SIMON_STATE_UP_STAIR_LEFT)
		ani = SIMON_ANI_LEFT_UP_STAIR;
	else
	if (state== SIMON_STATE_DOWN_STAIR_LEFT)
		ani = SIMON_ANI_LEFT_DOWN_STAIR;
	else
	if (state == SIMON_STATE_DOWN_STAIR_RIGHT)
		ani = SIMON_ANI_RIGHT_DOWN_STAIR;
	else
	if (state == SIMON_STATE_QUY) {
		if (nx>0) ani = SIMON_ANI_QUY_RIGHT;
		else ani = SIMON_ANI_QUY_LEFT;

	}
	else
	if (vy < 0) {
		if (nx>0) ani = SIMON_ANI_JUMP_RIGHT;
		else ani = SIMON_ANI_JUMP_LEFT;
	}
	else
	if (vx == 0)
	{
		if (nx > 0) { 
			if (untouchable == 3 ) {
				ani = SIMON_ANI_INVI_RIGHT;
			}
			else {
				ani = SIMON_ANI_IDLE_RIGHT;
			}
		}
		else {
			if (untouchable == 3) {
				ani = SIMON_ANI_INVI_LEFT;
			}
			else { 
				ani = SIMON_ANI_IDLE_LEFT; 
			}
		}
	}
	else if (vx > 0)
		ani = SIMON_ANI_WALKING_RIGHT;
	else ani = SIMON_ANI_WALKING_LEFT;
	

	int alpha = 255;
	if (untouchable) alpha = 128;
	animations[ani]->Render(x - cameraPosX, y - cameraPosY, alpha);
	
	//RenderBoundingBox();
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		vy = -SIMON_JUMP_SPEED_Y;
	case SIMON_STATE_IDLE:
		vx = 0;
		break;
	case SIMON_STATE_DIE:
		vy = 0;
		vx = 0;
		break;
	case SIMON_STATE_QUY:
		vx = 0;
		break;
	case SIMON_STATE_UP_STAIR_IDLE:
		vx = 0;
		vy = 0;
		break;
	case SIMON_STATE_DOWN_STAIR_IDLE:
		vx = 0;
		vy = 0;
		break;
	case SIMON_STATE_UP_STAIR_RIGHT:
		nx = 1;
		break;
	case SIMON_STATE_DOWN_STAIR_RIGHT:
		nx = 1;
		break;
	case SIMON_STATE_UP_STAIR_LEFT:
		nx = -1;
		break;
	case SIMON_STATE_DOWN_STAIR_LEFT:
		nx = -1;
		break;
	}
}

void CSimon::Attack(CMorningStar* morningStar, float cameraPosX, float cameraPosY, int attackType)
{
	if (attackType == 0) {
		if (!attacking) {
			attacking = 1;
			attack_start = GetTickCount();
			//this->animations
			//reset all attack animation fram
			animations[SIMON_ANI_ATTACK_RIGHT]->Reset();
			animations[SIMON_ANI_ATTACK_LEFT]->Reset();
			/*animations[SIMON_ANI_QUY_ATTACK_RIGHT]->Reset();
			animations[SIMON_ANI_QUY_ATTACK_LEFT]->Reset();*/
			morningStar->animations[MS_ANI_LV1_LEFT]->Reset();
			morningStar->animations[MS_ANI_LV1_RIGHT]->Reset();
			morningStar->animations[MS_ANI_LV2_LEFT]->Reset();
			morningStar->animations[MS_ANI_LV2_RIGHT]->Reset();
			morningStar->animations[MS_ANI_LV3_LEFT]->Reset();
			morningStar->animations[MS_ANI_LV3_RIGHT]->Reset();
			morningStar->is_last_frame = 0;	

		}
	}
	else {
		if (!attacking) {
			attacking = 2;
			attack_start = GetTickCount();
			//this->animations
			//reset all attack animation fram
			animations[SIMON_ANI_ATTACK_RIGHT]->Reset();
			animations[SIMON_ANI_ATTACK_LEFT]->Reset();


		}

	}
}

void CSimon::GoOnStair()
{
	if (nearUpStair && !onStair) {
		vx = 0;
		if (currentStair->nx > 0) {
			if (x >= currentStair->x - 1 && x <= currentStair->x + 1) {
				vy = 0;
				y -= 8;
				x = currentStair->x + 0.1;
				onStair = true;
				action = 0;
				nx = currentStair->nx;
			}
			else {
				if (x < currentStair->x - 1) {
					nx = 1;
					dx = SIMON_WALKING_SPEED / 2 * dt;
				}
				else {
					nx = -1;
					dx = -SIMON_WALKING_SPEED / 2 * dt;

				}

			}
		}
		else {
			if (x >= currentStair->x - 1 && x <= currentStair->x + 1) {
				vy = 0;
				y -= 8;
				x = currentStair->x + 0.1;
				onStair = true;
				action = 0;
				nx = currentStair->nx;
			}
			else {
				if (x < currentStair->x - 1) {
					nx = 1;
					dx = SIMON_WALKING_SPEED / 2 * dt;
				}
				else {
					nx = -1;
					dx = -SIMON_WALKING_SPEED / 2 * dt;

				}

			}

		}
		tempXforStair = 0;
		tempYforStair = 0;
	}
	if (nearDownStair && !onStair) {
		vx = 0;
		if (currentStair->nx > 0) {
			if (x >= currentStair->x + currentStair->width - SIMON_STAND_BBOX_WIDTH && x <= currentStair->x + currentStair->width - SIMON_STAND_BBOX_WIDTH + 1) {
				vy = 0;
				y += 8;
				x = currentStair->x - 0.1 + currentStair->width - SIMON_STAND_BBOX_WIDTH;
				onStair = true;
				action = 0;
				nx = (currentStair->nx > 0) ? -1 : 1;
			}
			else {
				if (x < currentStair->x - 1 + currentStair->width - SIMON_STAND_BBOX_WIDTH) {
					nx = 1;
					dx = SIMON_WALKING_SPEED / 2 * dt;
				}
				else {
					nx = -1;
					dx = -SIMON_WALKING_SPEED / 2 * dt;

				}

			}
		}
		else {
			if (x >= currentStair->x - currentStair->width + SIMON_STAND_BBOX_WIDTH -1 && x <= currentStair->x - currentStair->width + SIMON_STAND_BBOX_WIDTH) {
				vy = 0;
				y += 8;
				x = currentStair->x - 0.1 - currentStair->width + SIMON_STAND_BBOX_WIDTH;
				onStair = true;
				action = 0;
				nx = (currentStair->nx > 0) ? -1 : 1;
			}
			else {
				if (x < currentStair->x - currentStair->width + SIMON_STAND_BBOX_WIDTH) {
					nx = 1;
					dx = SIMON_WALKING_SPEED / 2 * dt;
				}
				else {
					nx = -1;
					dx = -SIMON_WALKING_SPEED / 2 * dt;

				}

			}
		}
		tempXforStair = 0;
		tempYforStair = 0;
	}
}


void CSimon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + SIMON_STAND_BBOX_WIDTH;
	bottom = y + SIMON_STAND_BBOX_HEIGHT;

}

void CSimon::takeDame(int nx)
{
	
	untouchable_start = GetTickCount();
	if (nx > 0) {
		untouchable = 1;
		vy = -0.2;
	}
	else {
		untouchable = -1;
		vy = -0.2;
	}
}

