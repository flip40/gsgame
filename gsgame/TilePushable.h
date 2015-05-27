#pragma once
#include "Tile.h"

class TilePushable : public Tile
{
	bool hasColumn;
	class Column* column;
public:
	TilePushable(int x, int y, int z, GLuint image, bool walkable);
	~TilePushable();
	bool isWalkable(class Character* character) const;
	void animate(class Character* character, class Map* map, float deltaTime);
	void setColumn(Column* column);
};

