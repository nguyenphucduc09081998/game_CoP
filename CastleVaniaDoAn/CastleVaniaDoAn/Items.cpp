#include "Items.h"

CItems::~CItems()
{

}
void CItems::Remove()
{
	this->x = -1000; // remove from sence
	state = STATE_REMOVED;
}
void CItems::Render(float & cameraPosX, float & cameraPosY)
{
	animations[0]->Render(x - cameraPosX, y - cameraPosY);
	RenderBoundingBox(cameraPosX, cameraPosY);
}

void CItems::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
void CItems::Update(DWORD dt, vector<LPGAMEOBJECT> *ItemColiObjects) {
	CGameObject::Update(dt);
	vy += ITEM_GRAVITY;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(ItemColiObjects, coEvents);
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
		x += min_tx * dx + nx * 0.1f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.1f;
		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

CItems::CItems(int itemType, float x, float y)
{
	{
		type = GO_TYPE_ITEM;
		this->x = x;
		this->y = y;
		item_type = itemType;
		switch (itemType) {
		case SMALL_HEART_ID:
			AddAnimation(7000);
			SetSize(8, 8);
			break;
		case BIG_HEART_ID:
			AddAnimation(7001);
			SetSize(12, 10);
			break;
		case BAG_1_ID:
			AddAnimation(70020);
			SetSize(15, 15);
			break;
		case BAG_2_ID:
			AddAnimation(70021);
			SetSize(15, 15);
			break;
		case BAG_3_ID:
			AddAnimation(70022);
			SetSize(15, 15);
			break;
		case MORNING_STAR_ID:
			AddAnimation(7003);
			SetSize(16, 16);
			break;
		case SWORD_ID:
			AddAnimation(7004);
			SetSize(16, 9);
			break;
		case CLOCK_ID:
			AddAnimation(7005);
			SetSize(15, 16);
			break;
		case NECKLET_ID:
			AddAnimation(7006);
			SetSize(16, 16);
			break;
		case AXE_ID:
			AddAnimation(7007);
			SetSize(15, 14);
			break;
		case FIRE_BOTTLE_ID:
			AddAnimation(7008);
			SetSize(16, 16);
			break;
		case CHICKEN_ID:
			AddAnimation(7009);
			SetSize(16, 13);
			break;
		case INVI_POTION_ID:
			AddAnimation(7010);
			SetSize(15, 18);
			break;
		}

	}
}
