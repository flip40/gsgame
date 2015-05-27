#include"TileJumpable.h"
#include"Character.h"

TileJumpable::TileJumpable(int x, int y, int z, GLuint image, bool walkable) : Tile(x, y, z, image, walkable) {
}

TileJumpable::~TileJumpable() {
}

void TileJumpable::animate(Character* character, Map* map, float deltaTime) {
	bool shouldJump = false;
	if (character->movingUp()) {
		if (map->getTile(x, y - 1, z)->isWalkable(character)) {
			shouldJump = true;
		}
	}
	else if (character->movingDown()) {
		if (map->getTile(x, y + 1, z)->isWalkable(character)) {
			shouldJump = true;
		}
	}
	else if (character->movingLeft()) {
		if (map->getTile(x - 1, y, z)->isWalkable(character)) {
			shouldJump = true;
		}
	}
	else if (character->movingRight()) {
		if (map->getTile(x + 1, y, z)->isWalkable(character)) {
			shouldJump = true;
		}
	}
	if (shouldJump)	character->jump(deltaTime);
}