#pragma once
#include<glew.h>
#include<iostream>
#include<fstream>

using namespace std;

class Map {
	GLuint tileart[28]; //This will likely also be a vector. Don't want this number hardcoded...
	class Tile* tiles[60][60][3]; //this will change to vector eventually
	int width, height, depth, leftEdge, topEdge, rightEdge, bottomEdge, level;
public:
	Map(int level);
	void draw(class Camera* camera); //draw all tiles
	void draw(int depth, class Camera* camera); //draw tiles at a specific depth
	bool containsBox(float x1, float y1, float x2, float y2);
	int getWidth();
	int getHeight();
	bool isWalkable(float x1, float y1, class Character* character);
	class Tile* getTile(int x, int y, int z);
	void setLevel(int level);
	int getDepth();
	int getLevel();
//	friend istream& operator >> (istream& in, Map& map);
//	friend ostream& operator << (ostream& out, Map& map);
private:
	bool hasTile(int x, int y, int z);
};