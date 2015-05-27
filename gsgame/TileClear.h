#pragma once
#include "Tile.h"

class TileClear : public Tile
{
public:
	TileClear(int x, int y, int z, GLuint image, bool walkable);
	~TileClear();
	void draw(float camX, float camY);
};

