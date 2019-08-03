#include "Brick.h"

CBrick::CBrick(int brickType, float x, float y, int item_drop) {
	this->type = GO_TYPE_BRICK;
	this->x = x;
	this->y = y;
	this->item_drop = item_drop;
	switch (brickType) {
	case 0:
		this->AddAnimation(200);
		width = 16;
		height = 16;
		break;
	case 1:
		this->AddAnimation(201);
		width = 16;
		height = 16;
		break;
	case 2:
		this->AddAnimation(202);
		width = 8;
		height = 8;
		break;
	case 3:
		this->AddAnimation(203);
		width = 16;
		height = 16;
		break;
	case 4:
		this->AddAnimation(204);
		width = 16;
		height = 16;
		break;
	}
}

LPCITEM CBrick::DropItem()
{
	CItems *item;
	item = new CItems(item_drop,_x,y);
	return item;
}

void CBrick::Render(float &cameraPosX, float &cameraPosY)
{
	animations[0]->Render(x- cameraPosX, y- cameraPosY);
	RenderBoundingBox(cameraPosX, cameraPosY);
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

void CBrick::SetState(int state) {
	this->state = state;
	if (state == STATE_REMOVED) {
		_x = x;
		//x = -1000;
	}

}