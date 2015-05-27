#include "TileClear.h"

TileClear::TileClear(int x, int y, int z, GLuint image, bool walkable) : Tile(x, y, z, image, walkable) {
}

TileClear::~TileClear() {
}

void TileClear::draw(float camX, float camY) {

}