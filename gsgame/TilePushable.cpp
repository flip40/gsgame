#include "TilePushable.h"
#include "Character.h"
#include "Map.h"
#include "Column.h"
#include <typeinfo>

TilePushable::TilePushable(int x, int y, int z, GLuint image, bool walkable) : Tile(x, y, z, image, walkable) {
}

TilePushable::~TilePushable() {
}

bool TilePushable::isWalkable(Character* character) const {
	if (hasColumn) {
		return false;
	}
	else {
		return true;
	}
}

void TilePushable::animate(Character* character, Map* map, float deltaTime) {
	if (hasColumn) {
		if (character->movingUp()) {
			Tile* tile = map->getTile(x, y - 1, z);
			if (typeid(*tile) == typeid(TilePushable)) {
				((TilePushable*)tile)->setColumn(column);
				map->getTile(x, y - 3, z + 2)->setWalkable(true);
				map->getTile(x, y - 2, z + 2)->setWalkable(false);
				column->moveUp();
			}
			else {
				return;
			}
		}
		else if (character->movingDown()) {
			Tile* tile = map->getTile(x, y + 1, z);
			if (typeid(*tile) == typeid(TilePushable)) {
				((TilePushable*)tile)->setColumn(column);
				map->getTile(x, y - 1, z + 2)->setWalkable(true);
				map->getTile(x, y - 2, z + 2)->setWalkable(false);
				column->moveDown();
			}
			else {
				return;
			}
		}
		else if (character->movingLeft()) {
			Tile* tile = map->getTile(x - 1, y, z);
			if (typeid(*tile) == typeid(TilePushable)) {
				((TilePushable*)tile)->setColumn(column);
				map->getTile(x - 1, y - 2, z + 2)->setWalkable(true);
				map->getTile(x, y - 2, z + 2)->setWalkable(false);
				column->moveLeft();
			}
			else {
				return;
			}
		}
		else if (character->movingRight()) {
			Tile* tile = map->getTile(x + 1, y, z);
			if (typeid(*tile) == typeid(TilePushable)) {
				((TilePushable*)tile)->setColumn(column);
				map->getTile(x + 1, y - 2, z + 2)->setWalkable(true);
				map->getTile(x, y - 2, z + 2)->setWalkable(false);
				column->moveRight();
			}
			else {
				return;
			}
		}

		hasColumn = false;
		column = NULL;
		character->push(deltaTime);
	}
}

void TilePushable::setColumn(Column* column) {
	hasColumn = true;
	this->column = column;
}