#include "Drawable.h"


Drawable::Drawable(float y, int z) {
	this->y = y;
	this->z = z;
}


Drawable::~Drawable() {
}

float Drawable::getY() const {
	return y;
}

int Drawable::getZ() const {
	return z;
}

bool sortByY(const Drawable* drawable1, const Drawable* drawable2) {
//	float d1y = drawable1->getY();
//	float d2y = drawable2->getY();
//	int d1z = drawable1->getZ();
//	int d2z = drawable2->getZ();
	if (drawable1->getZ() < drawable2->getZ()) {
		return true;
	}
	else if (drawable1->getZ() > drawable2->getZ()) {
		return false;
	}
	else if (drawable1->getY() < drawable2->getY()) {
		return true;
	}
	else {
		return false;
	}
}