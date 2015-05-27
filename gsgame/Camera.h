#pragma once
#include"Map.h"
#include"Drawable.h"
#include<vector>

using namespace std;

class Camera {
	float x, y;
	int width, height;
	class Map* map;
	class Character* character;
public:
	Camera(int width, int height);
	void setPos(int x, int y);
	void move(float dx, float dy, class Map* map);
	void centerOnPC(class Character* character, class Map* map);
	void draw(class Map* map, vector<class Drawable*>& drawables);
	float getX();
	float getY();
	float getX2();
	float getY2();
private:
	bool validPos(float x, float y, class Map* map);
};
