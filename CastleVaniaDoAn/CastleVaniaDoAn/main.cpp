#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <fstream>
#include <string>
#include "Stage.h"
#include "UI.h"


using namespace std;
int score = 0;
int current_stage = 1;
float cameraPosX = 0;
float cameraPosY = 0;
CSimon *Simon;
CMorningStar *MorningStar;
vector<LPSTAGE> stages;
vector<LPGAMEOBJECT> allGameObject;
CGame *game;
CUI *UI;
CStage *stage0;
CStage *stage1;
CStage *stage2;
CStage *stage3;
CStage *stage4;
CStage *stage5;


class CSampleKeyHander : public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

CSampleKeyHander * keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	switch (KeyCode)
	{
	case DIK_S:
		Simon->SetState(SIMON_STATE_JUMP);
		break;
	case DIK_R: // reset
		Simon->SetState(SIMON_STATE_IDLE);
		Simon->SetPosition(50.0f, 0.0f);
		Simon->SetSpeed(0, 0);
		break;
	case DIK_A:
		if (game->IsKeyDown(DIK_UP)) {
			Simon->Attack(MorningStar, cameraPosX, cameraPosY, 1);
		}
		else {
			Simon->Attack(MorningStar, cameraPosX, cameraPosY, 0);
		}
		break;
	case DIK_N:
		current_stage++;
		break;
	case DIK_M:
		Simon->subWeapon++;
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{

}

void CSampleKeyHander::KeyState(BYTE *states)
{

	if (Simon->GetState() == SIMON_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT)) {
		if (Simon->onStair) {
			if (Simon->CurrentStairDirection() > 0) {
				Simon->SetState(SIMON_STATE_UP_STAIR_RIGHT);
			}
			else {
				Simon->SetState(SIMON_STATE_DOWN_STAIR_RIGHT);
			}
		}
		else {
			Simon->SetState(SIMON_STATE_WALKING_RIGHT);
		}
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		if (Simon->onStair) {
			if (Simon->CurrentStairDirection() > 0) {
				Simon->SetState(SIMON_STATE_DOWN_STAIR_LEFT);
			}
			else {
				Simon->SetState(SIMON_STATE_UP_STAIR_LEFT);
			}
		}
		else {
			Simon->SetState(SIMON_STATE_WALKING_LEFT);
		}
	}
	else if (game->IsKeyDown(DIK_DOWN)) {
		if (Simon->nearDownStair && !Simon->onStair) {
			Simon->action = SIMON_ACTION_GO_ON_STAIR;

		}
		else
			if (Simon->onStair) {
				if (Simon->CurrentStairDirection() > 0) {
					Simon->SetState(SIMON_STATE_DOWN_STAIR_LEFT);
				}
				else {
					Simon->SetState(SIMON_STATE_DOWN_STAIR_RIGHT);
				}
			}
			else {
				Simon->SetState(SIMON_STATE_QUY);
			}
	}
	else if (game->IsKeyDown(DIK_UP)) {
		if (Simon->nearUpStair && !Simon->onStair) {
			//Simon->SetState(SIMON_STATE_GO_ON_STAIR);
			Simon->action = SIMON_ACTION_GO_ON_STAIR;
		}
		else
			if (Simon->onStair) {
				if (Simon->CurrentStairDirection() > 0) {
					Simon->SetState(SIMON_STATE_UP_STAIR_RIGHT);
				}
				else {
					Simon->SetState(SIMON_STATE_UP_STAIR_LEFT);
				}
			}
	}

	else
		if (Simon->onStair) {
			if (Simon->CurrentStairDirection() > 0) {
				if (Simon->nx > 0) {
					Simon->SetState(SIMON_STATE_UP_STAIR_IDLE);
				}
				else {
					Simon->SetState(SIMON_STATE_DOWN_STAIR_IDLE);
				}
			}
			else {
				if (Simon->nx < 0) {
					Simon->SetState(SIMON_STATE_UP_STAIR_IDLE);
				}
				else {
					Simon->SetState(SIMON_STATE_DOWN_STAIR_IDLE);
				}
			}
		}
		else {
			Simon->SetState(SIMON_STATE_IDLE);
		}
}


vector<string> split(const string str)
{
	vector<string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(" ", prev);
		string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + 1;
	} while (pos < str.length() && prev < str.length());
	return tokens;
}
void LoadSprite(CTextures * textures, CSprites * sprites, CAnimations * animations) {
	ifstream inFile;
	inFile.open(L"LoadResources\\Sprites.txt");
	if (inFile.is_open()) {
		string line;
		int currentTexId = -1;
		while (getline(inFile, line)) {
			if (line == "") {
				continue;
			}
			vector<string> results = split(line);
			if (results[0] == "//") {
				continue;

			}
			int action = stoi(results[0]);

			if (action == LOADSPRITE_ACTION_LOAD_FILE) {
				//format: action texId file_path
				int texId = stoi(results[1]);
				string fileName = results[2];
				//đổi lại định data type cho file name
				wstring stemp = wstring(fileName.begin(), fileName.end());
				LPCWSTR formatedFileName = stemp.c_str();
				//add texture
				textures->Add(texId, formatedFileName, D3DCOLOR_XRGB(255, 0, 255));
				currentTexId = texId;
			}
			if (action == LOADSPRITE_ACTION_ADD_SPRITE) {
				//format: action sprite_id left top right bottom ....
				int spriteId = stoi(results[1]);
				int left = stoi(results[2]);
				int top = stoi(results[3]);
				int right = stoi(results[4]);
				int bottom = stoi(results[5]);
				LPDIRECT3DTEXTURE9 textureHandle = textures->Get(currentTexId);
				sprites->Add(spriteId, left, top, right, bottom, textureHandle);
			}
			if (action == LOADSPRITE_ACTION_ADD_ANIMATION) {
				//format: action animation_id anmation_time sprite_id_1 sprite_id_2 sprite_id_3 ....
				int animationID = stoi(results[1]);
				int animationTime = stoi(results[2]);
				LPANIMATION ani;
				ani = new CAnimation(animationTime);
				for (int i = 3; i < results.size(); i++) {
					ani->Add(stoi(results[i]));
				}
				animations->Add(animationID, ani);
			}
		}
	}

}
void LoadResourcesStage(int stage, vector<int>& StageAllObjects, vector<LPGAMEOBJECT>& SimonColiObjects, vector<LPGAMEOBJECT>& CreepColiObjects, vector<LPGAMEOBJECT>& MicsColiObjects, vector<LPGAMEOBJECT>& WeaponColiObjects, vector<LPGAMEOBJECT>& ItemColiObjects, vector<LPCSTAIR>& StageStairs)
{
	ifstream inFile;
	switch (stage) {
	case 1:
		inFile.open(L"LoadResources\\Stage1.txt");
		break;
	case 2:
		inFile.open(L"LoadResources\\Stage2.txt");
		break;
	case 3:
		inFile.open(L"LoadResources\\Stage3.txt");
		break;
	case 4:
		inFile.open(L"LoadResources\\Stage4.txt");
		break;
	case 5:
		inFile.open(L"LoadResources\\Stage5.txt");
		break;
	}
	if (inFile.is_open()) {
		string line;
		while (getline(inFile, line)) {
			if (line == "") {
				continue;
			}
			vector<string> results = split(line);

			int ObjectType = stoi(results[0]);
			if (ObjectType == GO_TYPE_BRICK) {
				int brickType = stoi(results[1]);
				int x = stoi(results[2]);
				int y = stoi(results[3]);
				int item_drop = stoi(results[4]);
				CBrick * brick = new CBrick(brickType, x, y, item_drop);
				brick->id = allGameObject.size();
				allGameObject.push_back(brick);
				SimonColiObjects.push_back(brick);
				CreepColiObjects.push_back(brick);
				WeaponColiObjects.push_back(brick);
				StageAllObjects.push_back(brick->id);
				ItemColiObjects.push_back(brick);
			}
			if (ObjectType == GO_TYPE_MICS) {
				int micsType = stoi(results[1]);
				int item_drop = stoi(results[2]);
				int x = stoi(results[3]);
				int y = stoi(results[4]);
				CMics *mics = new CMics(micsType, item_drop, x, y);
				mics->id = allGameObject.size();
				allGameObject.push_back(mics);
				WeaponColiObjects.push_back(mics);
				StageAllObjects.push_back(mics->id);
			}
			if (ObjectType == GO_TYPE_ZOMBIE) {
				int x = stoi(results[1]);
				int y = stoi(results[2]);
				int nx = stoi(results[3]);
				CZombie *zomebie = new CZombie(x, y, nx);
				zomebie->id = allGameObject.size();
				allGameObject.push_back(zomebie);
				StageAllObjects.push_back(zomebie->id);
				if (nx < 0) {
					SimonColiObjects.push_back(zomebie);
					WeaponColiObjects.push_back(zomebie);
				}

			}
			if (ObjectType == GO_TYPE_DOG) {
				int x = stoi(results[1]);
				int y = stoi(results[2]);
				int nx = stoi(results[3]);
				int distance_before_jump = stoi(results[4]);
				CDog *zomebie = new CDog(x, y, nx, distance_before_jump);
				zomebie->id = allGameObject.size();
				allGameObject.push_back(zomebie);
				StageAllObjects.push_back(zomebie->id);
				SimonColiObjects.push_back(zomebie);
				WeaponColiObjects.push_back(zomebie);
			}
			if (ObjectType == GO_TYPE_BAT) {
				int x = stoi(results[1]);
				int y = stoi(results[2]);
				int nx = stoi(results[3]);
				CBat * bat = new CBat(x, y, nx);
				bat->id = allGameObject.size();
				allGameObject.push_back(bat);
				StageAllObjects.push_back(bat->id);
				if (nx < 0) {
					SimonColiObjects.push_back(bat);
					WeaponColiObjects.push_back(bat);
				}

			}
			if (ObjectType == GO_TYPE_FISH) {
				int x = stoi(results[1]);
				int y = stoi(results[2]);
				int nx = stoi(results[3]);
				CFish * fish = new CFish(x, y, nx);
				fish->id = allGameObject.size();
				allGameObject.push_back(fish);
				StageAllObjects.push_back(fish->id);
				if (nx < 0) {
					SimonColiObjects.push_back(fish);
					WeaponColiObjects.push_back(fish);
				}


			}
			if (ObjectType == GO_TYPE_STAIR) {
				int x = stoi(results[1]);
				int y = stoi(results[2]);
				int nx = stoi(results[3]);
				int width = stoi(results[4]);
				CStair *stair = new CStair(x, y, nx, width);
				StageStairs.push_back(stair);
			}
			if (ObjectType == GO_TYPE_BOSS) {
				int x = stoi(results[1]);
				int y = stoi(results[2]);
				CBoss *boss = new CBoss(600, 40);
				boss->id = allGameObject.size();
				allGameObject.push_back(boss);
				StageAllObjects.push_back(boss->id);
				SimonColiObjects.push_back(boss);
				WeaponColiObjects.push_back(boss);
			}

		}
		inFile.close();
	}

}
void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();
	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	LPANIMATION ani;
	//add sprite
	//enemy
	//zomebie
	/*textures->Add(ID_TEX_ENEMY_ZOMBIE, L"Textures\\enemy\\1.png", D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 texZombie = textures->Get(ID_TEX_ENEMY_ZOMBIE);
	sprites->Add(6000, 0, 0, 16, 32, texZombie);
	sprites->Add(6001, 18, 0, 33, 32, texZombie);
	sprites->Add(6002, 34, 0, 49, 32, texZombie);
	sprites->Add(6003, 52, 0, 67, 32, texZombie);
	ani = new CAnimation(100);
	ani->Add(6000);
	ani->Add(6001);
	animations->Add(6000, ani);
	ani = new CAnimation(100);
	ani->Add(6002);
	ani->Add(6003);
	animations->Add(6001, ani);*/
	LoadSprite(textures, sprites, animations);

	//UI
	UI = new CUI(game->GetDirect3DDevice());

	//Stage 1 resource
	vector<int> StageAllObjects;
	vector<LPGAMEOBJECT> SimonColiObjects;
	vector<LPGAMEOBJECT> CreepColiObjects;
	vector<LPGAMEOBJECT> MicsColiObjects;
	vector<LPGAMEOBJECT> WeaponColiObjects;
	vector<LPGAMEOBJECT> ItemColiObjects;

	//Simon
	//textures->Add(ID_TEX_SIMON, L"Textures\\simon.png", D3DCOLOR_XRGB(255, 0, 255));
	//dùng vòng for để add simon texture
	//LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);
	//int count = 1;
	//for (int i = 0; i < 3; i++) {
	//	for (int u = 0; u < 16; u++) {
	//		sprites->Add(1000 + count, u * 30 + 5, i * 33, (u + 1) * 30, (i + 1) * 33, texSimon);
	//		count++;
	//	}
	//}

	Simon = new CSimon();
	MorningStar = new CMorningStar();

	//Hòn đá vô nghĩa vói id 0
	CBrick *blablabrick = new CBrick(0, -100, -100, -1);
	blablabrick->id = allGameObject.size();
	allGameObject.push_back(blablabrick);





	//Stage 1 resource
	ifstream inFile;
	inFile.open(L"Textures\\titledMap\\tilemap1.txt");
	int value;
	vector<int> temp1;
	vector<vector<int>> Stage1Pos;
	while (inFile >> value) {
		temp1.push_back(value);
		if (temp1.size() == STAGE1_TEXT_NO) {
			Stage1Pos.push_back(temp1);
			temp1.clear();
		}
	}
	textures->Add(ID_TEX_BACKGROUND_1, L"Textures\\titledMap\\lv1.png", D3DCOLOR_XRGB(255, 255, 255));
	LPDIRECT3DTEXTURE9 texBackGround = textures->Get(ID_TEX_BACKGROUND_1);
	vector<LPSPRITE> Stage1Sprite;
	for (int i = 0; i < 47; i++) {
		sprites->Add(30000 + i, i * 16, 0, (i + 1) * 16, 16, texBackGround);
		Stage1Sprite.push_back(sprites->Get(30000 + i));
	}
	CBackGround *background = new CBackGround(Stage1Sprite, Stage1Pos, 0, 40);

	vector<LPCSTAIR> Stage1Stairs;
	LoadResourcesStage(1, StageAllObjects, SimonColiObjects, CreepColiObjects, MicsColiObjects, WeaponColiObjects, ItemColiObjects, Stage1Stairs);
	stage1 = new CStage(1, allGameObject, StageAllObjects, SimonColiObjects, CreepColiObjects, MicsColiObjects, WeaponColiObjects, ItemColiObjects, Simon, MorningStar, background, STAGE_1_WIDTH, Stage1Stairs);
	stages.push_back(stage1);
	stages.push_back(stage1);
	//end stage 1 load

	//Stage 2 resource
	vector<int> StageAllObjects2;
	vector<LPGAMEOBJECT> SimonColiObjects2;
	vector<LPGAMEOBJECT> CreepColiObjects2;
	vector<LPGAMEOBJECT> MicsColiObjects2;
	vector<LPGAMEOBJECT> WeaponColiObjects2;
	vector<LPGAMEOBJECT> ItemColiObjects2;
	//BackgGround
	ifstream inFile2;
	inFile2.open(L"Textures\\titledMap\\tilemap2.txt");
	int value2;

	vector<int> temp2;
	vector<vector<int>> Stage2Pos;
	while (inFile2 >> value2) {
		temp2.push_back(value2);
		if (temp2.size() == STAGE2_TEXT_NO) {
			Stage2Pos.push_back(temp2);
			temp2.clear();
		}
	}

	textures->Add(ID_TEX_BACKGROUND_2, L"Textures\\titledMap\\lv2.png", D3DCOLOR_XRGB(255, 255, 255));
	LPDIRECT3DTEXTURE9 texBackGround2 = textures->Get(ID_TEX_BACKGROUND_2);
	//Sprites id for background will be start from 3000
	vector<LPSPRITE> Stage2Sprite;
	for (int i = 0; i <= 100; i++) {
		sprites->Add(32000 + i, i * 16, 0, (i + 1) * 16, 16, texBackGround2);
		Stage2Sprite.push_back(sprites->Get(32000 + i));
	}


	CBackGround *background2 = new CBackGround(Stage2Sprite, Stage2Pos, 0, 40);
	vector<LPCSTAIR> Stage2Stairs;
	LoadResourcesStage(2, StageAllObjects2, SimonColiObjects2, CreepColiObjects2, MicsColiObjects2, WeaponColiObjects2, ItemColiObjects2, Stage2Stairs);
	stage2 = new CStage(2, allGameObject, StageAllObjects2, SimonColiObjects2, CreepColiObjects2, MicsColiObjects2, WeaponColiObjects2, ItemColiObjects2, Simon, MorningStar, background2, STAGE_2_WIDTH, Stage2Stairs);
	stages.push_back(stage2);

	//end stage 2 load

	//Stage 3 resource
	vector<int> StageAllObjects3;
	vector<LPGAMEOBJECT> SimonColiObjects3;
	vector<LPGAMEOBJECT> CreepColiObjects3;
	vector<LPGAMEOBJECT> MicsColiObjects3;
	vector<LPGAMEOBJECT> WeaponColiObjects3;
	vector<LPGAMEOBJECT> ItemColiObjects3;
	vector<LPCSTAIR> Stage3Stairs;
	//BackgGround
	ifstream inFile3;
	inFile3.open(L"Textures\\titledMap\\tilemap3.txt");
	int value3;
	vector<int> temp3;
	vector<vector<int>> Stage3Pos;
	while (inFile3 >> value3) {
		temp3.push_back(value3);
		if (temp3.size() == STAGE3_TEXT_NO) {
			Stage3Pos.push_back(temp3);
			temp3.clear();
		}
	}
	CBackGround *background3 = new CBackGround(Stage2Sprite, Stage3Pos, 0, 40);
	LoadResourcesStage(3, StageAllObjects3, SimonColiObjects3, CreepColiObjects3, MicsColiObjects3, WeaponColiObjects3, ItemColiObjects3, Stage3Stairs);
	stage3 = new CStage(3, allGameObject, StageAllObjects3, SimonColiObjects3, CreepColiObjects3, MicsColiObjects3, WeaponColiObjects3, ItemColiObjects3, Simon, MorningStar, background3, STAGE_3_WIDTH, Stage3Stairs);
	stages.push_back(stage3);

	//stage 4
	vector<int> StageAllObjects4;
	vector<LPGAMEOBJECT> SimonColiObjects4;
	vector<LPGAMEOBJECT> CreepColiObjects4;
	vector<LPGAMEOBJECT> MicsColiObjects4;
	vector<LPGAMEOBJECT> WeaponColiObjects4;
	vector<LPGAMEOBJECT> ItemColiObjects4;
	vector<LPCSTAIR> Stage4Stairs;
	//background
	ifstream inFile4;
	inFile4.open(L"Textures\\titledMap\\tilemap4.txt");
	int value4;
	vector<int> temp4;
	vector<vector<int>> Stage4Pos;
	while (inFile4 >> value4) {
		temp4.push_back(value4);
		if (temp4.size() == STAGE4_TEXT_NO) {
			Stage4Pos.push_back(temp4);
			temp4.clear();
		}
	}
	textures->Add(ID_TEX_BACKGROUND_4, L"Textures\\titledMap\\lv4.png", D3DCOLOR_XRGB(255, 255, 255));
	LPDIRECT3DTEXTURE9 texBackGround4 = textures->Get(ID_TEX_BACKGROUND_4);
	vector<LPSPRITE> Stage4Sprite;
	for (int i = 0; i < 27; i++) {
		sprites->Add(34000 + i, i * 16, 0, (i + 1) * 16, 16, texBackGround4);
		Stage4Sprite.push_back(sprites->Get(34000 + i));
	}
	CBackGround *background4 = new CBackGround(Stage4Sprite, Stage4Pos, 0, 40);
	LoadResourcesStage(4, StageAllObjects4, SimonColiObjects4, CreepColiObjects4, MicsColiObjects4, WeaponColiObjects4, ItemColiObjects4, Stage4Stairs);
	stage4 = new CStage(4, allGameObject, StageAllObjects4, SimonColiObjects4, CreepColiObjects4, MicsColiObjects4, WeaponColiObjects4, ItemColiObjects4, Simon, MorningStar, background4, STAGE_4_WIDTH, Stage4Stairs);
	stages.push_back(stage4);


	//stage 5
	vector<int> StageAllObjects5;
	vector<LPGAMEOBJECT> SimonColiObjects5;
	vector<LPGAMEOBJECT> CreepColiObjects5;
	vector<LPGAMEOBJECT> MicsColiObjects5;
	vector<LPGAMEOBJECT> WeaponColiObjects5;
	vector<LPGAMEOBJECT> ItemColiObjects5;
	vector<LPCSTAIR> Stage5Stairs;
	//background
	ifstream inFile5;
	inFile5.open(L"Textures\\titledMap\\tilemap5.txt");
	int value5;
	vector<int> temp5;
	vector<vector<int>> Stage5Pos;
	while (inFile5 >> value5) {
		temp5.push_back(value5);
		if (temp5.size() == STAGE5_TEXT_NO) {
			Stage5Pos.push_back(temp5);
			temp5.clear();
		}
	}
	CBackGround *background5 = new CBackGround(Stage2Sprite, Stage5Pos, 0, 40);
	LoadResourcesStage(5, StageAllObjects5, SimonColiObjects5, CreepColiObjects5, MicsColiObjects5, WeaponColiObjects5, ItemColiObjects5, Stage5Stairs);
	stage5 = new CStage(5, allGameObject, StageAllObjects5, SimonColiObjects5, CreepColiObjects5, MicsColiObjects5, WeaponColiObjects5, ItemColiObjects5, Simon, MorningStar, background5, STAGE_5_WIDTH, Stage5Stairs);
	stages.push_back(stage5);

}
void Update(DWORD dt)
{

	stages[current_stage]->Update(dt, allGameObject, cameraPosX, cameraPosY, score);
	UI->Update(Simon);
	UI->stage = current_stage;
	UI->enemy_heal = stages[current_stage]->bossHeal;
	UI->score = score;
	if (stages[current_stage]->ended == true) {
		int lastStage = current_stage;
		current_stage = stages[current_stage]->nextStage;
		if (current_stage == 2) {
			Simon->SetPosition(5, 168);
			Simon->SetSpeed(0, 0);
			cameraPosX = 0;
		}
		if (current_stage == 3) {
			stages[4]->ended = false;
			if (stages[lastStage]->flag1 == 0) {
				Simon->SetPosition(40, 72.9);
				cameraPosX = 0;
			}
			else
				if (stages[lastStage]->flag1 == 1) {
					Simon->SetPosition(50, 180);
				}
				else
					if (stages[lastStage]->flag1 == 2) {
						Simon->SetPosition(370, 180);
					}
		}
		if (current_stage == 4) {
			stages[3]->ended = false;
			if (stages[lastStage]->flag1 == 1) {
				Simon->SetPosition(380, 32);
			}
			else {
				stages[3]->ended = false;
				Simon->SetPosition(60, 32);
				cameraPosX = 0;
			}

		}
		if (current_stage == 5) {
			Simon->SetPosition(24, 72.9);
			cameraPosX = 0;
		}
	}
}


/*
Render a frame
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);


		//
		if (stages[current_stage]->DestroyEnemy) {
			if (GetTickCount() % 2 == 1) {
				d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR_WHITE);
			}
			else {
				stages[current_stage]->Render(cameraPosX, cameraPosY, allGameObject);
			}
		}
		else {
			stages[current_stage]->Render(cameraPosX, cameraPosY, allGameObject);
		}
		UI->Render();
		spriteHandler->End();
		d3ddv->EndScene();
	}


	d3ddv->Present(NULL, NULL, NULL, NULL);
}
//create window
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}
int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);


	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}

