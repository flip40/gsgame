#include"Camera.h"
#include"Character.h"

Camera::Camera(int width, int height) {
	this->x = 0;
	this->y = 0;
	this->width = width;
	this->height = height;
}

void Camera::setPos(int x, int y) {
	this->x = x;
	this->y = y;
}

void Camera::move(float dx, float dy, Map* map) {
	float tempX = x + dx;
	float tempY = y + dy;
	if (validPos(tempX, tempY, map)) {
		x = tempX;
		y = tempY;
	}
	else {
		if (tempX < 0) x = 0;
		else if ((tempX + width) > map->getWidth()) x = map->getWidth() - width;
		if (tempY < 0) y = 0;
		else if ((tempY + height) > map->getHeight()) y = map->getHeight() - height;
	}
}

void Camera::centerOnPC(Character* character, Map* map) {
	float tempX = character->getX() + (character->getWidth() / 2) - (width / 2);
	float tempY = character->getY() + (character->getHeight() / 2) - (height / 2);
	if (validPos(tempX, 0, map)) {
		x = tempX;
	}
	if (validPos(0, tempY, map)) {
		y = tempY;
	}
}

void Camera::draw(Map* map, vector<Drawable*>& drawables) {
	int drawable = 0; //keep track of how many drawables we have drawn thus far
	for(int i = 0; i < map->getDepth(); i++) { //for each level...
		map->draw(i, this); //draw current level of tiles
		while (drawable < drawables.size() && drawables[drawable]->getZ() < i) { //draw chars below (lags behind by 1 level)
			drawables[drawable]->draw(this);
			drawable++;
		}
	}
	while (drawable < drawables.size()) { //draw remaining chars (top level)
		drawables[drawable]->draw(this);
		drawable++;
	}
}

float Camera::getX() {
	return x;
}

float Camera::getY() {
	return y;
}

float Camera::getX2() {
	return x + width;
}

float Camera::getY2() {
	return y + height;
}

bool Camera::validPos(float x, float y, Map* map) {
	return map->containsBox(x, y, x + width, y + height);
}
