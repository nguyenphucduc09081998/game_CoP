#pragma once
#include "UI.h"
#include <string>


void CUI::DrawTextString(int x, int y, wchar_t *str)
{

	SetRect(&font_rect, x, y, 100, 100);

	g_font->DrawText(NULL,        //pSprite
		str,  //pString
		-1,          //Count
		&font_rect,  //pRect
		DT_LEFT | DT_NOCLIP,//Format,
		0xFFFFFFFF); //Color


}

CUI::CUI(LPDIRECT3DDEVICE9 d3ddv)
{
	this->AddAnimation(2000);
	this->AddAnimation(2001);
	this->AddAnimation(2002);
	this->AddAnimation(2003);
	this->AddAnimation(7004);
	this->AddAnimation(7005);
	this->AddAnimation(7006);
	this->AddAnimation(7007);
	this->AddAnimation(7008);
	r = D3DXCreateFont(d3ddv,
		7,
		0,
		FW_NORMAL,
		1,
		false,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DRAFT_QUALITY,
		DEFAULT_PITCH,
		L"Arcade Normal",
		&g_font);

	score = 0;
	time = 300;
	stage = 1;
	player_heal = 16;
	enemy_heal = 16;
	heart = 0;
	life = 3;
	lastTick = GetTickCount();
}

void CUI::AddAnimation(int aniId)
{
	LPANIMATION ani = CAnimations::GetInstance()->Get(aniId);
	animations.push_back(ani);
}

void CUI::Render()
{
	

	animations[0]->Render(142, 16);

	//60 24

		for (int i = 0; i < player_heal; i++) {
			animations[1]->Render(55 + i*5, 16);
		}
		for (int u = player_heal; u < 16; u++) {
			animations[2]->Render(55 + u * 5, 16);

		}
		for (int i = 0; i < enemy_heal; i++) {
			animations[3]->Render(55 + i * 5, 25);
		}
		for (int u = enemy_heal; u < 16; u++) {
			animations[2]->Render(55 + u * 5, 25);

		}
	
	wstring score_text = L"SCORE-";
	wstring score_number = to_wstring(score).c_str();
	while (score_number.size() < 6) {
		score_number.insert(0, L"0");
	}
	score_text += score_number;
	wchar_t* score = &score_text[0];
	DrawTextString(2, 8, score);
	DrawTextString(2, 16, L"PLAYER");
	DrawTextString(2, 24, L"ENEMY");


	wstring time_text = L"TIME ";
	wstring time_number = to_wstring(time).c_str();
	while (time_number.size() < 3) {
		time_number.insert(0, L"0");
	}
	time_text += time_number;
	wchar_t* time = &time_text[0];
	DrawTextString(105, 8, time);
	
	wstring stage_text = L"STAGE ";
	wstring stage_number = to_wstring(stage).c_str();
	while (stage_number.size() < 2) {
		stage_number.insert(0, L"0");
	}
	stage_text += stage_number;
	wchar_t* stage = &stage_text[0];
	DrawTextString(175, 8, stage);

	wstring heart_number = to_wstring(heart).c_str();
	while (heart_number.size() < 2) {
		heart_number.insert(0, L"0");
	}
	wchar_t* heart = &heart_number[0];
	DrawTextString(193, 16, heart);

	wstring life_number = to_wstring(life).c_str();
	while (life_number.size() < 2) {
		life_number.insert(0, L"0");
	}
	wchar_t* life = &life_number[0];
	DrawTextString(193, 24, life);

	if (subWeapon == 4) {

		animations[4]->Render(150, 22);
	}else
		if(subWeapon == 5) {

		animations[5]->Render(150, 22);
	}
		else
			if (subWeapon == 7) {

				animations[7]->Render(150, 22);
			}
			else
				if (subWeapon == 8) {

					animations[8]->Render(150, 22);
				}
}

void CUI::Update(CSimon *simon)
{
	this->heart = simon->heart;
	this->player_heal = simon->player_heal;
	if (GetTickCount() - lastTick > 1000) {
		lastTick = GetTickCount();
		time -= 1;
	}
	this->subWeapon = simon->subWeapon;
}
