#pragma once
#include "Camera.h"

class Drawable
{
protected:
	float y;
	int z;
public:
	Drawable(float y, int z);
	~Drawable();
	virtual void draw(class Camera* camera) = 0;
	float getY() const;
	int getZ() const;
};

bool sortByY(const class Drawable* drawable1, const class Drawable* drawable2);

