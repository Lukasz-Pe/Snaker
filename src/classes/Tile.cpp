/*
 * T39-Tile.cpp
 *
 *  Created on: 26.06.2018
 *      Author: Łukasz Pękalski
 */

#include "Tile.h"

Tile::Tile(int x, int y, int gTileDimX, int gTileDimY, int tileType) {
	mBox.x = x;
	mBox.y = y;
	mBox.w = gTileDimX;
	mBox.h = gTileDimY;
	mType = tileType;
}

void Tile::setNewPos(int &x, int &y) {
	mBox.x = x;
	mBox.y = y;
}

void Tile::render(SDL_Rect &gCamera, std::shared_ptr<Win> window, LTexture &vTexture, SDL_Rect *vClip) {
	if (vClip != NULL) {
		if (checkCollision(gCamera, mBox)) {
			vTexture.render(mBox.x - gCamera.x, mBox.y - gCamera.y, window, &vClip[mType]);
		}
	} else {
		vTexture.render(mBox.x - gCamera.x, mBox.y - gCamera.y, window);
	}
}

int Tile::getType() {
	return mType;
}
SDL_Rect Tile::getBox() {
	return mBox;
}

void Tile::setWindowCoords(int x, int y) {
	mBox.x = x;
	mBox.y = y;
}
