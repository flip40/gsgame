#pragma once
#include<glew.h>

class Tile {
	GLuint image;
	bool walkable;
protected:
	int x, y, z;
public:
	Tile(int x, int y, int z, GLuint image, bool walkable);
	virtual void draw(float camX, float camY);
	virtual bool isWalkable(class Character* character) const;
	virtual void animate(class Character* character, class Map* map, float deltaTime);
	virtual void walkedOn(class Character* character, class Map* map, float deltaTime);
	void setWalkable(bool isWalkable);
//	friend istream& operator >> (istream& in, Tile& tile);
//	friend ostream& operator << (ostream& out, Tile& tile);
};
