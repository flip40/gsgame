#include "TileExit.h"
#include "Map.h"
#include "Character.h"

TileExit::TileExit(int x, int y, int z, GLuint image, bool walkable, int toLevel, float toX, float toY, float toZ) : Tile(x, y, z, image, walkable) {
	this->toLevel = toLevel;
	this->toX = toX;
	this->toY = toY;
	this->toZ = toZ;
}

TileExit::~TileExit() {
}

void TileExit::walkedOn(Character* character, Map* map, float deltaTime) {
	character->setPos(toX, toY, toZ);
	map->setLevel(toLevel);
}