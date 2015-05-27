#pragma once
#include "Tile.h"

class TileJumpable : public Tile
{
public:
	TileJumpable(int x, int y, int z, GLuint image, bool walkable);
	~TileJumpable();
	void animate(class Character* character, class Map* map, float deltaTime);
};

