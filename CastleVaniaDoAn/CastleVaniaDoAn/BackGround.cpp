#include "BackGround.h"

void RoundNumber(float &number) {
	int temp = number * 10000;
	number = temp / 10000;
}

void CBackGround::Render(float cameraPosX, float cameraPosY)
{
	int fromIndex = (cameraPosX - MODIFY_WIDTH) / CELL_WIDTH;
	int toIndex = (cameraPosX + MODIFY_WIDTH + SCREEN_WIDTH) / CELL_WIDTH;
	fromIndex = fromIndex < 0 ? 0 : fromIndex;
	toIndex = toIndex > position[0].size()-1 ? position[0].size()-1 : toIndex;
	for (int i = 0; i < position.size(); i++) {
		for (int u = fromIndex; u <= toIndex; u++) {
			float drawX = x + u * CELL_WIDTH - cameraPosX;
			float drawY = y + i * CELL_WIDTH - cameraPosY;
			RoundNumber(drawX);
			RoundNumber(drawY);
			sprites[position[i][u]]->Draw(drawX, drawY, 255);

		}

	}
}


