#pragma once
#include "Tile.h"

class TileExit : public Tile
{
	int toLevel;
	float toX, toY, toZ;
public:
	TileExit(int x, int y, int z, GLuint image, bool walkable, int toLevel, float toX, float toY, float toZ);
	~TileExit();
	void walkedOn(class Character* character, class Map* map, float deltaTime);
};

