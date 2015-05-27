#include "TileLadder.h"
#include "Character.h"

TileLadder::TileLadder(int x, int y, int z, GLuint image, bool walkable) : Tile(x, y, z, image, walkable) {
}


TileLadder::~TileLadder() {
}

bool TileLadder::isWalkable(Character* character) const {
	if (character->isClimbing()) {
		return true;
	}
	else {
		return false;
	}
}

void TileLadder::animate(Character* character, Map* map, float deltaTime) {
	character->startClimbing(deltaTime);
}