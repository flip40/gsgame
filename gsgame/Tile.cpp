#include<glew.h>
#include<DrawUtils.h>
#include"Tile.h"
#include"Character.h"

Tile::Tile(int x, int y, int z, GLuint image, bool walkable) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->image = image;
	this->walkable = walkable;
}

void Tile::draw(float camX, float camY) {
	if (image) {
		int drawX = (x * 16) - (int)camX;
		int drawY = (y * 16) - (int)camY;
		glDrawSprite(image, drawX, drawY, 16, 16);
	}
}

bool Tile::isWalkable(Character* character) const {
	if (walkable && character->isClimbing()) {
		return false;
	}
	return this->walkable;
}

void Tile::animate(Character* character, Map* map, float deltaTime) {
	if (walkable && character->isClimbing()) character->stopClimbing(deltaTime);
}

void Tile::setWalkable(bool isWalkable) {
	this->walkable = isWalkable;
}

void Tile::walkedOn(Character* character, Map* map, float deltaTime) {

}

//istream& operator >> (istream& in, Tile& tile) {
//	in >> map.depth;
//	in >> map.tiles;
//	return in;
//}
//
//ostream& operator << (ostream& out, Tile& tile) {
//	out << map.depth;
//	return out;
//}

