#include "Stage.h"
#include <ctime>

CStage::CStage(int stage, vector<LPGAMEOBJECT> allGameObject, vector<int> stageObjects, vector<LPGAMEOBJECT> SimonColiObjects, vector<LPGAMEOBJECT> CreepColiObjects, vector<LPGAMEOBJECT> MicsColiObjects, vector<LPGAMEOBJECT> WeaponColiObjects, vector<LPGAMEOBJECT> ItemColiObjects, CSimon *Simon, CMorningStar *MorningStar, CBackGround *BackGround, int stageWidth, vector<LPCSTAIR> stairs)
{
	currentZone = 0;
	this->stage = stage;
	this->SimonColiObjects	= SimonColiObjects;
	this->CreepColiObjects	= CreepColiObjects;
	this->MicsColiObjects	= MicsColiObjects;
	this->WeaponColiObjects = WeaponColiObjects;
	this->ItemColiObjects	= ItemColiObjects;
	this->stageWidth		= stageWidth;
	this->Simon				= Simon;
	this->MorningStar		= MorningStar;
	this->BackGround		= BackGround;
	noZone				= stageWidth / ZONE_WIDTH;
	noZone					= (ZONE_WIDTH* noZone < stageWidth) ? noZone + 1 : noZone;
	ended					= false;
	for (int i = 0; i < noZone; i++) {
		vector<int> tmpObjects;
		for (int u = 0; u < stageObjects.size(); u++) {
			int  gameObjectId  = stageObjects[u];
			CGameObject *object = allGameObject[gameObjectId];
			if (object->x >= i * ZONE_WIDTH && object->x < ( i+1 ) * ZONE_WIDTH) {
				tmpObjects.push_back(gameObjectId);
				object->belongZone = i;
				object->originalZone = i;
			}
			
		}
		ZonesObjects.push_back(tmpObjects);
	}
	this->stairs = stairs;
}

void CStage::Update(DWORD dt, vector<LPGAMEOBJECT> &allGameObject, float & cameraPosX, float & cameraPosY, int& score)
{
	if (DestroyEnemy) {
		if (GetTickCount() > stopEnemyStart + 800) {
			DestroyEnemy = false;
		}
	}else {
		if (stopEnemy) {
			if (GetTickCount() > stopEnemyStart + STOP_ENEMY_LASTING) {
				stopEnemy = false;

			}

		}
		//change scence
		switch (stage) {
		case 1:
			if (Simon->x > 675) { // change scence position
				Simon->vx = 0.075; //to run animaton
				Simon->x += dt * Simon->vx;
				if (Simon->x > 704) {
					nextStage = 2;
					ended = true;
				}
				return;
			}
			break;
		case 2:

			if (Simon->x > 1506 && cameraPosX < 1548 - SCREEN_WIDTH / 2) {
				cameraPosX += 0.05*dt;
				return;
			}
			if (Simon->x > 1506 && Simon->x < 1576) {
				
				// chạy gate animation xong thì chạy simon animation
				Simon->vx = 0.075;
				Simon->x += Simon->vx *dt;
				// đổi cảnh luôn
				return;
			}
			if (Simon->x > 1576 && cameraPosX < 1536) {

				cameraPosX += 0.05*dt;
				return;
			}
			else {
				if (Simon->x > 1576 && cameraPosX > 1536) {
					ended = true;
					nextStage = 3;
					return;
				}
			}
			break;
		case 3:
			if (Simon->y > 192) {
				ended = true;
				nextStage = 4;
				if (Simon->x > 100) {
					flag1 = 1;
				}
				return;
			}
			if (Simon->x > 486 && Simon->y < 100 && cameraPosX < 512 - SCREEN_WIDTH / 2) {
				cameraPosX += 0.05*dt;
				return;
			}
			if (Simon->x > 486 && Simon->x < 536 && Simon->y < 100) {
				// chạy gate animation xong thì chạy simon animation
				Simon->vx = 0.075;
				Simon->x += Simon->vx *dt;
				// đổi cảnh luôn
				return;
			}
			if (Simon->x > 536 && cameraPosX < 512 && Simon->y < 100) {

				cameraPosX += 0.05*dt;
				return;
			}
			if (Simon->x > 536 && cameraPosX > 512 && Simon->y < 100) {
				flag1 = 2;
				nextStage = 5;
				ended = true;
				return;
			}
			break;
		case 4:
			if (Simon->y < 29 && Simon->onStair) {
				if (Simon->x < 80) {
					flag1 = 1;
					nextStage = 3;
					ended = true;
					return;
				}
				else {
					flag1 = 2;
					nextStage = 3;
					ended = true;
					return;
				}
			}
			if (Simon->y > 170 && Simon->state != SIMON_STATE_DIE) {
				CEffect* effect1;
				effect1 = new CEffect(2, Simon->x - 5, Simon->y, -0.03, 0.1, 1);
				this->Insert_gameObject(effect1, allGameObject);
				CEffect* effect2;
				effect2 = new CEffect(2, Simon->x + 5, Simon->y, 0.04, 0.1, 1);
				this->Insert_gameObject(effect2, allGameObject);
				CEffect* effect3;
				effect3 = new CEffect(2, Simon->x - 2, Simon->y, -0.04, 0.1, 1);
				this->Insert_gameObject(effect3, allGameObject);
				CEffect* effect4;
				effect4 = new CEffect(2, Simon->x + 2, Simon->y, 0.03, 0.1, 1);
				this->Insert_gameObject(effect4, allGameObject);
				Simon->player_heal = 0;
				Simon->state = SIMON_STATE_DIE;
			}
			break;


		}
		//update simon
		Simon->Update(dt, cameraPosX, cameraPosY, stageWidth, MorningStar, &SimonColiObjects, stairs, score);


		if (Simon->destroyEnemy) {
			Simon->destroyEnemy = false;
			DestroyEnemy = true;
			stopEnemyStart = GetTickCount();
		}
		//special handle camera stage 2
		if (stage == 2 && cameraPosX + SCREEN_WIDTH > 1548 && Simon->x < 1506) {
			cameraPosX = 1548 - SCREEN_WIDTH;
		}


		//update morning star
		if (Simon->attacking == 1) {
			MorningStar->Update(dt, Simon->x, Simon->y, Simon->nx, &WeaponColiObjects);
		}
		//update subweapon
		if (Simon->attacking == 2 && Simon->isLastFrame && Simon->created_subweapon == 0) {
			if (Simon->subWeapon == 4) {
				CSubWeapon *sword = new CSubWeapon(4, Simon->nx);
				sword->SetPosition(Simon->x + 2, Simon->y + 2);
				SubWeapon_objects.push_back(sword);
				Simon->created_subweapon = 1;
			}
			if (Simon->subWeapon == 5) {
				if (!stopEnemy) {
					stopEnemy = true;
					stopEnemyStart = GetTickCount();
				}
			}
			if (Simon->subWeapon == 7) {
				CSubWeapon *axe = new CSubWeapon(7, Simon->nx);
				axe->SetPosition(Simon->x + 2, Simon->y + 2);
				SubWeapon_objects.push_back(axe);
				Simon->created_subweapon = 1;
			}
			if (Simon->subWeapon == 8) {
				CSubWeapon *sword = new CSubWeapon(8, Simon->nx);
				sword->SetPosition(Simon->x + 2, Simon->y + 2);
				SubWeapon_objects.push_back(sword);
				Simon->created_subweapon = 1;
			}

		}
		//update zone
			//đi về trước
		if (cameraPosX > ZONE_WIDTH*(currentZone + 1)) {
			currentZone++;
		}
		////đi lùi
		if (cameraPosX < ZONE_WIDTH * currentZone && currentZone != 0) {
			currentZone--;

		}
		for (int u = currentZone - 1; u <= currentZone + 2; u++) {
			if (u >= 0 && u < noZone) {
				for (int i = 0; i < ZonesObjects[u].size(); i++) {
					if (ZonesObjects[u][i] == 0)
					{
						continue;

					}
					//update bricks
					if (allGameObject[ZonesObjects[u][i]]->type == 2) {
						CBrick* brick = dynamic_cast<CBrick*>(allGameObject[ZonesObjects[u][i]]);
						if (brick->state == STATE_REMOVED) {
							if (brick->item_drop != BRICK_UNABLE_DROP_ITEM) {
								CItems* item = brick->DropItem();
								this->Insert_gameObject(item, allGameObject);
							}

							for (int i = 0; i < 3; i++) {

							}

							CEffect* effect1;
							effect1 = new CEffect(1, brick->_x - 5, brick->y + 200, -0.03, 0.1, 1);
							this->Insert_gameObject(effect1, allGameObject);
							CEffect* effect2;
							effect2 = new CEffect(1, brick->_x + 5, brick->y + 20, 0.04, 0.1, 1);
							this->Insert_gameObject(effect2, allGameObject);
							CEffect* effect3;
							effect3 = new CEffect(1, brick->_x - 2, brick->y + 20, -0.04, 0.1, 1);
							this->Insert_gameObject(effect3, allGameObject);
							CEffect* effect4;
							effect4 = new CEffect(1, brick->_x + 2, brick->y + 20, 0.03, 0.1, 1);
							this->Insert_gameObject(effect4, allGameObject);


							brick->x = -100;
							ZonesObjects[u][i] = 0;
							ZonesObjects[u].erase(ZonesObjects[u].begin() + i);
						}
					}

					if (allGameObject[ZonesObjects[u][i]]->type == 6 && allGameObject[ZonesObjects[u][i]]->belongZone == u) {
						CEnemy* Enemy = dynamic_cast<CEnemy*>(allGameObject[ZonesObjects[u][i]]);
						if (DestroyEnemy) {
							Enemy->SetState(STATE_BEING_REMOVE);
						}
						if (Enemy->GetState() == STATE_REMOVED) {
							score += 100;
							
							CItems* item = Enemy->DropItem();
							if (item != NULL) {
								this->Insert_gameObject(item, allGameObject);
							}
							Enemy->x = -100;
							ZonesObjects[u][i] = 0;
						}
						else {
							if (Enemy->canUpdate) {
								float simonX;
								float simonY;
								Simon->GetPosition(simonX, simonY);
								Enemy->Update(dt, &CreepColiObjects, simonX, stopEnemy);

								//fish fire
								if (Enemy->enemyType == 4) {
									if (Enemy->attackStart + FISH_ATTACK_TIME < GetTickCount() && Enemy->canAttack) {
										Enemy->canAttack = false;
										CEffect* bullet = new CEffect(Enemy->x, Enemy->y+4, BULLET_SPEED, 0, (int)Enemy->nx);
										this->Insert_gameObject(bullet, allGameObject);
										WeaponColiObjects.push_back(bullet);
										SimonColiObjects.push_back(bullet);
									}
									//bắn nước khi xuất hiện
									if (Enemy->y < 150 && Enemy->action == 0) {
										CEffect* effect1;
										effect1 = new CEffect(2, Enemy->x - 5, Enemy->y, -0.03, 0.1, 1);
										this->Insert_gameObject(effect1, allGameObject);
										CEffect* effect2;
										effect2 = new CEffect(2, Enemy->x + 5, Enemy->y, 0.04, 0.1, 1);
										this->Insert_gameObject(effect2, allGameObject);
										CEffect* effect3;
										effect3 = new CEffect(2, Enemy->x - 2, Enemy->y, -0.04, 0.1, 1);
										this->Insert_gameObject(effect3, allGameObject);
										CEffect* effect4;
										effect4 = new CEffect(2, Enemy->x + 2, Enemy->y, 0.03, 0.1, 1);
										this->Insert_gameObject(effect4, allGameObject);
										Enemy->action = 1;
									}

									//bán nước khi rớt
									if (Enemy->y > 170 && Enemy->action == 1) {
										CEffect* effect1;
										effect1 = new CEffect(2, Enemy->x - 5, Enemy->y, -0.03, 0.1, 1);
										this->Insert_gameObject(effect1, allGameObject);
										CEffect* effect2;
										effect2 = new CEffect(2, Enemy->x + 5, Enemy->y, 0.04, 0.1, 1);
										this->Insert_gameObject(effect2, allGameObject);
										CEffect* effect3;
										effect3 = new CEffect(2, Enemy->x - 2, Enemy->y, -0.04, 0.1, 1);
										this->Insert_gameObject(effect3, allGameObject);
										CEffect* effect4;
										effect4 = new CEffect(2, Enemy->x + 2, Enemy->y, 0.03, 0.1, 1);
										this->Insert_gameObject(effect4, allGameObject);
										Enemy->action = 0;
									}
								}


								//change zone for enemy
								if (Enemy->x < Enemy->belongZone * ZONE_WIDTH && Enemy->belongZone != 0) {
									Enemy->belongZone -= 1;

									ZonesObjects[u][i] = 0;
									ZonesObjects[Enemy->belongZone].push_back(Enemy->id);


								}
								else {
									if (Enemy->x >= (Enemy->belongZone + 1) * ZONE_WIDTH && Enemy->belongZone != noZone - 1) {
										if (Enemy->x < cameraPosX + SCREEN_WIDTH) {
											Enemy->belongZone += 1;

											ZonesObjects[u][i] = 0;
											ZonesObjects[Enemy->belongZone].push_back(Enemy->id);
										}
										else {
											Enemy->x = -100;
											ZonesObjects[u][i] = 0;
										}
									

									}
								}
								if (Enemy->x > stageWidth) {
									ZonesObjects[u][i] = 0;
								}
							}
							else {
								srand(time(NULL));
								if (Enemy->enemyType == 4) {
									if (Enemy->nx > 0) {
										if (Simon->x - Enemy->x > 80 && Enemy->y - Simon->y < 110) {
											Enemy->canUpdate = true;
											SimonColiObjects.push_back(Enemy);
											WeaponColiObjects.push_back(Enemy);
										}
									}
									else {
										if (Enemy->x - Simon->x < (rand() % 3 + 3) * 20 && Enemy->y - Simon->y < 110) {
											Enemy->canUpdate = true;
											SimonColiObjects.push_back(Enemy);
											WeaponColiObjects.push_back(Enemy);
										}
									}

								}
								else {
									if (cameraPosX - Enemy->x > (rand() % 6) * 20 && Simon->y - Enemy->y<20 && Simon->y - Enemy->y>-20) {
										Enemy->canUpdate = true;
										SimonColiObjects.push_back(Enemy);
										WeaponColiObjects.push_back(Enemy);
									}
								}
							}
						}
					}
					//update mics
					if (allGameObject[ZonesObjects[u][i]]->type == 5) {
						if (allGameObject[ZonesObjects[u][i]]->GetState() == STATE_REMOVED) {
							CMics* Mic = dynamic_cast<CMics*>(allGameObject[ZonesObjects[u][i]]);
							CItems* item = Mic->DropItem();
							this->Insert_gameObject(item, allGameObject);
							ZonesObjects[u].erase(ZonesObjects[u].begin() + i);
						}
						else {
							allGameObject[ZonesObjects[u][i]]->Update(dt, &MicsColiObjects);
						}
					}
					//update item
					if (allGameObject[ZonesObjects[u][i]]->type == 10) {
						allGameObject[ZonesObjects[u][i]]->Update(dt, &ItemColiObjects);
					}
					//update item effect
					if (allGameObject[ZonesObjects[u][i]]->type == GO_TYPE_EFFECT_BULLET || allGameObject[ZonesObjects[u][i]]->type == GO_TYPE_EFFECT) {
						CEffect* effect = dynamic_cast<CEffect*>(allGameObject[ZonesObjects[u][i]]);
						if (allGameObject[ZonesObjects[u][i]]->GetState() == STATE_REMOVED) {
							allGameObject[ZonesObjects[u][i]]->x = -100;
							ZonesObjects[u][i] = 0;
						}
						else {
							effect->Update(dt);

							if (effect->x < effect->belongZone * ZONE_WIDTH && effect->belongZone != 0) {
								effect->belongZone -= 1;
								if (effect->belongZone != effect->originalZone) {
									ZonesObjects[u][i] = 0;
									ZonesObjects[effect->belongZone].push_back(effect->id);
								}

							}
							if (effect->x >= (effect->belongZone + 1) * ZONE_WIDTH && effect->belongZone != noZone - 1) {
								effect->belongZone += 1;
								if (effect->belongZone != effect->originalZone) {
									ZonesObjects[u][i] = 0;
									ZonesObjects[effect->belongZone].push_back(effect->id);
								}
							}
						}
					}
					if (allGameObject[ZonesObjects[u][i]]->type == GO_TYPE_BOSS) {
						CBoss* boss = dynamic_cast<CBoss*>(allGameObject[ZonesObjects[u][i]]);
						if (boss->GetState() == STATE_REMOVED) {
							//CItems* item = Mic->DropItem();
							//this->Insert_gameObject(item, allGameObject);
							boss->x = -100;
							ZonesObjects[u].erase(ZonesObjects[u].begin() + i);
						}
						else {
							boss->Update(dt, Simon, stopEnemy);

							if (boss->state == BOSS_STATE_RANGE_ATTACK && boss->canAttack) {
								float distance = sqrt((boss->x - Simon->x)*(boss->x - Simon->x) + (boss->y - Simon->y)*(boss->y - Simon->y));
								CEffect* bullet = new CEffect(boss->x + 22, boss->y + 4, BULLET_SPEED*((Simon->x - boss->x) / distance), BULLET_SPEED*((Simon->y - boss->y) / distance), Simon->x - boss->x);
								this->Insert_gameObject(bullet, allGameObject);
								WeaponColiObjects.push_back(bullet);
								SimonColiObjects.push_back(bullet);
								boss->canAttack = false;
							}
						}
						bossHeal = boss->bossHeal;
					}
				}
			}
		}
		//subweapon (special handle)
		for (int i = 0; i < SubWeapon_objects.size(); i++) {
			SubWeapon_objects[i]->Update(dt, &WeaponColiObjects);
			if (SubWeapon_objects[i]->x < cameraPosX - 20 || SubWeapon_objects[i]->x > cameraPosX + SCREEN_WIDTH || SubWeapon_objects[i]->GetState() == STATE_REMOVED) {
				SubWeapon_objects.erase(SubWeapon_objects.begin() + i);

			}
		}
	}
	
}

void CStage::Render(float cameraPosX, float cameraPosY, vector<LPGAMEOBJECT> allGameObject)
{
	BackGround->Render(cameraPosX, cameraPosY);
	for (int u = currentZone - 1; u <= currentZone + 2; u++) {
		if (u >= 0 && u < noZone) {
		for (int i = 0; i < ZonesObjects[u].size(); i++) {
			if (ZonesObjects[u][i] == 0)
			{
				continue;
					
			}
			if (dynamic_cast<CEnemy*>(allGameObject[ZonesObjects[u][i]])) {
				CEnemy* enemy = dynamic_cast<CEnemy*>(allGameObject[ZonesObjects[u][i]]);
				if (enemy->canUpdate) {
					allGameObject[ZonesObjects[u][i]]->Render(cameraPosX, cameraPosY);
				}
			}
			else {
				allGameObject[ZonesObjects[u][i]]->Render(cameraPosX, cameraPosY);
			}
			}
		}
	}


	//render simon
	Simon->Render(cameraPosX, cameraPosY);
	//render morningstar
	if (Simon->attacking == 1) {
		MorningStar->Render(cameraPosX, cameraPosY, Simon->nx);
	}
	for (int i = 0; i < SubWeapon_objects.size(); i++) {
		SubWeapon_objects[i]->Render(cameraPosX, cameraPosY);

	}

	//render stage 1 change scence gate
	if (stage == 1 && Simon->x > 675) {
		float drawX = 704 - cameraPosX;

		int temp = drawX * 10000;
		drawX = temp / 10000;

		LPSPRITE GateSprite1 = CSprites::GetInstance()->Get(30015); //3000 is gate animation
		GateSprite1->Draw(drawX, 136 - cameraPosY); // gate positiond
		LPSPRITE GateSprite2 = CSprites::GetInstance()->Get(30024); //3000 is gate animation
		GateSprite2->Draw(drawX, 152 - cameraPosY); // gate positiond
		LPSPRITE GateSprite3 = CSprites::GetInstance()->Get(30045); //3000 is gate animation
		GateSprite3->Draw(drawX, 168 - cameraPosY); // gate positiond
		LPSPRITE GateSprite4 = CSprites::GetInstance()->Get(30010); //3000 is gate animation
		GateSprite4->Draw(drawX + 16, 136 - cameraPosY); // gate positiond
		LPSPRITE GateSprite5 = CSprites::GetInstance()->Get(30033); //3000 is gate animation
		GateSprite5->Draw(drawX + 16, 152 - cameraPosY); // gate positiond
		LPSPRITE GateSprite6 = CSprites::GetInstance()->Get(30046); //3000 is gate animation
		GateSprite6->Draw(drawX + 16, 168 - cameraPosY); // gate positiond
	}


}

void CStage::Insert_gameObject(LPGAMEOBJECT go, vector<LPGAMEOBJECT> &allGameObject)
{
	go->id = allGameObject.size();
	allGameObject.push_back(go);
	int belong_zone = go->x / ZONE_WIDTH;
	
	if (belong_zone >= 0 && belong_zone < noZone) {
		go->belongZone = belong_zone;
		this->ZonesObjects[belong_zone].push_back(go->id);
		if (go->type == GO_TYPE_ITEM) {
			this->SimonColiObjects.push_back(go);
		}
	}
}


CStage::~CStage()
{
}


