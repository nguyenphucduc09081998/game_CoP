#pragma once
#pragma once
class CStair
{


public:
	float width;
	float x;
	float y;
	int nx;
	CStair(float x, float y, int nx, float width) {
		this->x = x;
		this->y = y;
		this->nx = nx;
		this->width = width;
	};
	~CStair();
};

typedef CStair * LPCSTAIR;

