#pragma once
#include "Tile.h"

class TileLadder : public Tile
{
public:
	TileLadder(int x, int y, int z, GLuint image, bool walkable);
	~TileLadder();
	bool isWalkable(class Character* character) const;
	void animate(class Character* character, class Map* map, float deltaTime);
};

