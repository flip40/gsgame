#include<glew.h>
#include"DrawUtils.h"
#include"Map.h"
#include"Tile.h"
#include"TileClear.h"
#include"TileJumpable.h"
#include"TileLadder.h"
#include"TilePushable.h"
#include"TileExit.h"
#include"Camera.h"
#include<iostream>
#include"Character.h"

Map::Map(int level) {
	tileart[0] = glTexImageTGAFile("resources/maps/tile4.tga", NULL, NULL); // l1 tile
	tileart[1] = glTexImageTGAFile("resources/maps/tile2.tga", NULL, NULL); // block
	tileart[2] = glTexImageTGAFile("resources/maps/tile5.tga", NULL, NULL); // darkness wall
	tileart[3] = glTexImageTGAFile("resources/maps/tile6.tga", NULL, NULL); // air
	tileart[4] = glTexImageTGAFile("resources/maps/tile7.tga", NULL, NULL); // wall left
	tileart[5] = glTexImageTGAFile("resources/maps/tile8.tga", NULL, NULL); // wall right
	tileart[6] = glTexImageTGAFile("resources/maps/tile10.tga", NULL, NULL); // l2 bottom right
	tileart[7] = glTexImageTGAFile("resources/maps/tile11.tga", NULL, NULL); // l2 right
	tileart[8] = glTexImageTGAFile("resources/maps/tile12.tga", NULL, NULL); // l2 bottom
	tileart[9] = glTexImageTGAFile("resources/maps/tile13.tga", NULL, NULL); // l2 tile
	tileart[10] = glTexImageTGAFile("resources/maps/ladder1.tga", NULL, NULL); // ladder bottom
	tileart[11] = glTexImageTGAFile("resources/maps/ladder2.tga", NULL, NULL); // ladder middle
	tileart[12] = glTexImageTGAFile("resources/maps/ladder3.tga", NULL, NULL); // ladder top
	tileart[13] = glTexImageTGAFile("resources/maps/tile14.tga", NULL, NULL); // wall
	tileart[14] = glTexImageTGAFile("resources/maps/topleft.tga", NULL, NULL); // top left
	tileart[15] = glTexImageTGAFile("resources/maps/top.tga", NULL, NULL); // top
	tileart[16] = glTexImageTGAFile("resources/maps/topright.tga", NULL, NULL); // top right
	tileart[17] = glTexImageTGAFile("resources/maps/left.tga", NULL, NULL); // left
	tileart[18] = glTexImageTGAFile("resources/maps/bottomleft.tga", NULL, NULL); // bottom left
	tileart[19] = glTexImageTGAFile("resources/maps/bottom.tga", NULL, NULL); // bottom
	tileart[20] = glTexImageTGAFile("resources/maps/bottomright.tga", NULL, NULL); // bottom right
	tileart[21] = glTexImageTGAFile("resources/maps/right.tga", NULL, NULL); // right
	tileart[22] = glTexImageTGAFile("resources/maps/tile3.tga", NULL, NULL); // portal
	tileart[23] = glTexImageTGAFile("resources/maps/tile15.tga", NULL, NULL); // l2 left
	tileart[24] = glTexImageTGAFile("resources/maps/tile16.tga", NULL, NULL); // l2 top
	tileart[25] = glTexImageTGAFile("resources/maps/tile17.tga", NULL, NULL); // l2 top left
	tileart[26] = glTexImageTGAFile("resources/maps/tile18.tga", NULL, NULL); // l2 top right
	tileart[27] = glTexImageTGAFile("resources/maps/tile19.tga", NULL, NULL); // l2 bottom left

	this->setLevel(level);
}

void Map::draw(Camera* camera) {
	float camX = camera->getX();
	float camY = camera->getY();
	float camX2 = camera->getX2();
	float camY2 = camera->getY2();
	float xend = camX2 / 16;
	float yend = camY2 / 16;
	for (int i = camX/16; i < xend; i++) {
		for (int j = camY/16; j < yend; j++) {
			for (int k = 0; k < depth; k++) {
				if (tiles[i][j][k])	tiles[i][j][k]->draw(camX, camY);
			}
		}
	}
}

void Map::draw(int depth, Camera* camera) {
	float camX = camera->getX();
	float camY = camera->getY();
	float camX2 = camera->getX2();
	float camY2 = camera->getY2();
	float xend = camX2 / 16;
	float yend = camY2 / 16;
	for (int i = camX/16; i < xend; i++) {
		for (int j = camY/16; j < yend; j++) {
			if (tiles[i][j][depth])	tiles[i][j][depth]->draw(camX, camY);
		}
	}
}

bool Map::containsBox(float x1, float y1, float x2, float y2) {
	return (x1 >= leftEdge && y1 >= topEdge && x2 <= rightEdge && y2 <= bottomEdge);
}

int Map::getWidth() {
	return rightEdge;
}

int Map::getHeight() {
	return bottomEdge;
}

bool Map::isWalkable(float x, float y, Character* character) {
	float x1 = 0;
	float x2 = 0;
	float y1 = 0;
	float y2 = 0;
	if (character->isClimbing()) {
		x1 = x - 7;
		x2 = x + 7;
		y1 = y - 20;
		y2 = y + 2;
	}
	else {
		x1 = x - 7;
		x2 = x + 7;
		y1 = y - 3;
		y2 = y + 7;
	}
	//cout << "x: " << x << " y: " << y << "\n";

	if (x1 >= leftEdge && y1 >= topEdge && x2 <= rightEdge && y2 <= bottomEdge) {
		//This will later use the character circle collision, simplifying for now with a box
		//Scratch that, I think using the circle would have been easier Q.Q
		//Really not sure why this keeps ending up so clunky, I'll swap to circle when I have some time
		//Circles are nice, and separating axis theorem is nice. I think that will go much better
		//check if any tile is collidable, if so see if colliding (in 3x3 centered on char x,y)
		int tilex = (int)(x / 16);
		int tiley = (int)(y / 16);
		int tilez = character->getZ();
		//std::cout << tilex << "," << tiley << "\n";
		if (character->isClimbing()) {
			//bottom center
			if (hasTile(tilex, tiley, tilez) && !tiles[tilex][tiley][tilez]->isWalkable(character)) {
				int tilex1 = tilex * 16;
				int tiley1 = tiley * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//top center
			if (hasTile(tilex, tiley - 1, tilez) && !tiles[tilex][tiley - 1][tilez]->isWalkable(character)) {
				int tilex1 = tilex * 16;
				int tiley1 = (tiley - 1) * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//bottom
			if (hasTile(tilex, tiley + 1, tilez) && !tiles[tilex][tiley + 1][tilez]->isWalkable(character)) {
				int tilex1 = tilex * 16;
				int tiley1 = (tiley + 1) * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//top
			if (hasTile(tilex, tiley - 2, tilez) && !tiles[tilex][tiley - 2][tilez]->isWalkable(character)) {
				int tilex1 = tilex * 16;
				int tiley1 = (tiley - 2) * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//top right
			if (hasTile(tilex + 1, tiley - 2, tilez) && !tiles[tilex + 1][tiley - 2][tilez]->isWalkable(character)) {
				int tilex1 = (tilex + 1) * 16;
				int tiley1 = (tiley - 2) * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//top left
			if (hasTile(tilex - 1, tiley - 2, tilez) && !tiles[tilex - 1][tiley - 2][tilez]->isWalkable(character)) {
				int tilex1 = (tilex - 1) * 16;
				int tiley1 = (tiley - 2) * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//mid bottom left
			if (hasTile(tilex - 1, tiley, tilez) && !tiles[tilex - 1][tiley][tilez]->isWalkable(character)) {
				int tilex1 = (tilex - 1) * 16;
				int tiley1 = tiley * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//mid bottom right
			if (hasTile(tilex + 1, tiley, tilez) && !tiles[tilex + 1][tiley][tilez]->isWalkable(character)) {
				int tilex1 = (tilex + 1) * 16;
				int tiley1 = tiley * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//mid top right
			if (hasTile(tilex + 1, tiley - 1, tilez) && !tiles[tilex + 1][tiley - 1][tilez]->isWalkable(character)) {
				int tilex1 = (tilex + 1) * 16;
				int tiley1 = (tiley - 1) * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//mid top left
			if (hasTile(tilex - 1, tiley - 1, tilez) && !tiles[tilex - 1][tiley - 1][tilez]->isWalkable(character)) {
				int tilex1 = (tilex - 1) * 16;
				int tiley1 = (tiley - 1) * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//bottom right
			if (hasTile(tilex + 1, tiley + 1, tilez) && !tiles[tilex + 1][tiley + 1][tilez]->isWalkable(character)) {
				int tilex1 = (tilex + 1) * 16;
				int tiley1 = (tiley + 1) * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//bottom left
			if (hasTile(tilex - 1, tiley + 1, tilez) && !tiles[tilex - 1][tiley + 1][tilez]->isWalkable(character)) {
				int tilex1 = (tilex - 1) * 16;
				int tiley1 = (tiley + 1) * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
		}
		else {
			//center tile
			if (hasTile(tilex, tiley, tilez) && !tiles[tilex][tiley][tilez]->isWalkable(character)) {
				int tilex1 = tilex * 16;
				int tiley1 = tiley * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//top tile
			if (hasTile(tilex, tiley - 1, tilez) && !tiles[tilex][tiley - 1][tilez]->isWalkable(character)) {
				int tilex1 = tilex * 16;
				int tiley1 = (tiley - 1) * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//bottom tile
			if (hasTile(tilex, tiley + 1, tilez) && !tiles[tilex][tiley + 1][tilez]->isWalkable(character)) {
				int tilex1 = tilex * 16;
				int tiley1 = (tiley + 1) * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//left tile
			if (hasTile(tilex - 1, tiley, tilez) && !tiles[tilex - 1][tiley][tilez]->isWalkable(character)) {
				int tilex1 = (tilex - 1) * 16;
				int tiley1 = tiley * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//right tile
			if (hasTile(tilex + 1, tiley, tilez) && !tiles[tilex + 1][tiley][tilez]->isWalkable(character)) {
				int tilex1 = (tilex + 1) * 16;
				int tiley1 = tiley * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//topright
			if (hasTile(tilex + 1, tiley - 1, tilez) && !tiles[tilex + 1][tiley - 1][tilez]->isWalkable(character)) {
				int tilex1 = (tilex + 1) * 16;
				int tiley1 = (tiley - 1) * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//topleft
			if (hasTile(tilex - 1, tiley - 1, tilez) && !tiles[tilex - 1][tiley - 1][tilez]->isWalkable(character)) {
				int tilex1 = (tilex - 1) * 16;
				int tiley1 = (tiley - 1) * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//bottomright
			if (hasTile(tilex + 1, tiley + 1, tilez) && !tiles[tilex + 1][tiley + 1][tilez]->isWalkable(character)) {
				int tilex1 = (tilex + 1) * 16;
				int tiley1 = (tiley + 1) * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
			//bottomleft
			if (hasTile(tilex - 1, tiley + 1, tilez) && !tiles[tilex - 1][tiley + 1][tilez]->isWalkable(character)) {
				int tilex1 = (tilex - 1) * 16;
				int tiley1 = (tiley + 1) * 16;
				int tilex2 = tilex1 + 16;
				int tiley2 = tiley1 + 16;
				if (x1 < tilex2 && x2 > tilex1 && y1 < tiley2 && y2 > tiley1) {
					return false;
				}
			}
		}
		return true;
	}
	else {
		return false;
	}
}

Tile* Map::getTile(int x, int y, int z) {
	return tiles[x][y][z];
}

bool Map::hasTile(int x, int y, int z) {
	if (x >= 0 && x < width && y >= 0 && y < height && z >= 0 && z < depth) {
		return true;
	}
	else {
		return false;
	}
}

void Map::setLevel(int level) {
	this->level = level;
	for (int i = 0; i < 60; i++) {
		for (int j = 0; j < 60; j++) {
			for (int k = 0; k < 3; k++) {
				tiles[i][j][k] = new Tile(i, j, k, NULL, true);
			}
		}
	}

	if (level == 1) {
		this->width = 21;
		this->height = 21;
		this->depth = 3;
		this->leftEdge = 0;
		this->topEdge = 0;
		this->rightEdge = this->width * 16;
		this->bottomEdge = this->height * 16;

		//z = 0
		//base air
		for (int i = 0; i < 21; i++)
		{
			for (int j = 0; j < 21; j++)
			{
				tiles[i][j][0] = new Tile(i, j, 0, tileart[3], false);
			}
		}
		//center tower base
		for (int i = 6; i < 15; i++) {
			for (int j = 6; j < 15; j++) {
				tiles[i][j][0] = new Tile(i, j, 0, tileart[0], true);
			}
		}
		for (int i = 6; i < 15; i++) {
			tiles[i][15][0] = new Tile(i, 15, 0, tileart[2], false);
		}
		for (int i = 7; i < 14; i++) {
			for (int j = 6; j < 14; j++) {
				tiles[i][j][0] = new Tile(i, j, 0, tileart[3], false);
			}
		}
		//ladder
		tiles[10][13][0] = new TileLadder(10, 13, 0, tileart[10], false);
		//top left
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				tiles[i][j][0] = new Tile(i, j, 0, tileart[0], true);
			}
		}
		for (int i = 0; i < 5; i++) {
			tiles[i][5][0] = new Tile(i, 5, 0, tileart[2], false);
		}
		//top right
		for (int i = 16; i < 21; i++) {
			for (int j = 0; j < 5; j++) {
				tiles[i][j][0] = new Tile(i, j, 0, tileart[0], true);
			}
		}
		for (int i = 16; i < 21; i++) {
			tiles[i][5][0] = new Tile(i, 5, 0, tileart[2], false);
		}
		//bottom left
		for (int i = 0; i < 5; i++) {
			for (int j = 16; j < 21; j++) {
				tiles[i][j][0] = new Tile(i, j, 0, tileart[0], true);
			}
		}
		//bottom right
		for (int i = 16; i < 21; i++) {
			for (int j = 16; j < 21; j++) {
				tiles[i][j][0] = new Tile(i, j, 0, tileart[0], true);
			}
		}
		//left bridge
		for (int i = 5; i < 16; i++) {
			if (i % 2) {
				tiles[2][i][0] = new TileJumpable(2, i, 0, tileart[2], false);
			}
			else {
				tiles[2][i][0] = new Tile(2, i, 0, tileart[0], true);
			}
		}
		tiles[3][10][0] = new TileJumpable(3, 10, 0, tileart[3], false);
		tiles[4][10][0] = new Tile(4, 10, 0, tileart[0], true);
		tiles[4][11][0] = new Tile(4, 11, 0, tileart[2], false);
		tiles[5][10][0] = new TileJumpable(5, 10, 0, tileart[3], false);
		//right bridge
		for (int i = 5; i < 16; i++) {
			if (i % 2) {
				tiles[18][i][0] = new TileJumpable(18, i, 0, tileart[2], false);
			}
			else {
				tiles[18][i][0] = new Tile(18, i, 0, tileart[0], true);
			}
		}
		tiles[15][10][0] = new TileJumpable(15, 10, 0, tileart[3], false);
		tiles[16][10][0] = new Tile(16, 10, 0, tileart[0], true);
		tiles[16][11][0] = new Tile(16, 11, 0, tileart[2], false);
		tiles[17][10][0] = new TileJumpable(17, 10, 0, tileart[3], false);
		//bottom bridge
		for (int i = 5; i < 16; i++) {
			if (!(i % 2)) {
				tiles[i][18][0] = new Tile(i, 18, 0, tileart[0], true);
				tiles[i][19][0] = new Tile(i, 19, 0, tileart[2], true);
			}
			else {
				tiles[i][18][0] = new TileJumpable(i, 18, 0, tileart[3], false);
			}
		}
		tiles[10][15][0] = new TileJumpable(10, 15, 0, tileart[2], false);
		tiles[10][16][0] = new Tile(10, 16, 0, tileart[0], true);
		tiles[10][17][0] = new TileJumpable(10, 17, 0, tileart[2], false);

		//z = 1
		for (int i = 0; i < 21; i++) {
			for (int j = 0; j < 21; j++) {
				tiles[i][j][1] = new Tile(i, j, 1, NULL, false);
			}
		}
		//wall
		for (int i = 7; i < 14; i++) {
			for (int j = 10; j < 14; j++) {
				tiles[i][j][1] = new Tile(i, j, 1, tileart[13], false);
			}
		}
		//ladder
		tiles[10][13][1] = new TileLadder(10, 13, 1, tileart[10], false);
		tiles[10][12][1] = new TileLadder(10, 12, 1, tileart[10], false);
		tiles[10][11][1] = new TileLadder(10, 11, 1, tileart[10], false);
		tiles[10][10][1] = new TileLadder(10, 10, 1, tileart[11], false);
		//above ladder
		tiles[10][9][1] = new Tile(10, 9, 1, NULL, true);
		//below ladder
		tiles[10][14][1] = new Tile(10, 14, 1, NULL, true);

		//z = 2
		for (int i = 0; i < 21; i++) {
			for (int j = 0; j < 21; j++) {
				tiles[i][j][2] = new Tile(i, j, 2, NULL, false);
			}
		}
		//ladder
		tiles[10][10][2] = new TileLadder(10, 10, 2, tileart[10], false);
		//mid
		for (int i = 8; i < 13; i++) {
			for (int j = 5; j < 9; j++) {
				tiles[i][j][2] = new Tile(i, j, 2, tileart[9], true);
			}
		}
		//top
		for (int i = 7; i < 14; i++) {
			tiles[i][4][2] = new Tile(i, 4, 2, tileart[24], true);
		}
		//left
		for (int i = 4; i < 10; i++) {
			tiles[7][i][2] = new Tile(7, i, 2, tileart[23], true);
		}
		//bottom
		for (int i = 7; i < 14; i++) {
			tiles[i][9][2] = new Tile(i, 9, 2, tileart[8], true);
		}
		//right
		for (int i = 4; i < 10; i++) {
			tiles[13][i][2] = new Tile(13, i, 2, tileart[7], true);
		}
		//top left
		tiles[7][4][2] = new Tile(7, 4, 2, tileart[25], true);
		//top right
		tiles[13][4][2] = new Tile(13, 4, 2, tileart[26], true);
		//bottom left
		tiles[7][9][2] = new Tile(7, 9, 2, tileart[27], true);
		//bottom right
		tiles[13][9][2] = new Tile(13, 9, 2, tileart[6], true);
		
		//teleportal
		tiles[10][6][2] = new TileExit(10, 6, 2, tileart[22], true, 2, 168, 104, 0);
	}
	else if (level == 2) {
		this->width = 60;
		this->height = 21;
		this->depth = 3;
		this->leftEdge = 0;
		this->topEdge = 0;
		this->rightEdge = this->width * 16;
		this->bottomEdge = this->height * 16;

		for (int i = 0; i < 60; i++) {
			for (int j = 0; j < 21; j++) {
				tiles[i][j][0] = new Tile(i, j, 0, tileart[3], false);
			}
		}

		//left island
		//mid
		for (int i = 8; i < 13; i++) {
			for (int j = 5; j < 9; j++) {
				tiles[i][j][0] = new Tile(i, j, 0, tileart[9], true);
			}
		}
		//top
		for (int i = 7; i < 14; i++) {
			tiles[i][4][0] = new Tile(i, 4, 0, tileart[24], true);
		}
		//left
		for (int i = 4; i < 10; i++) {
			tiles[7][i][0] = new Tile(7, i, 0, tileart[23], true);
		}
		//bottom
		for (int i = 7; i < 14; i++) {
			tiles[i][9][0] = new Tile(i, 9, 0, tileart[8], true);
		}
		//bottom bottom
		for (int i = 7; i < 14; i++) {
			tiles[i][10][0] = new Tile(i, 10, 0, tileart[2], false);
		}
		//right
		for (int i = 4; i < 10; i++) {
			tiles[13][i][0] = new Tile(13, i, 0, tileart[7], true);
		}
		//top left
		tiles[7][4][0] = new Tile(7, 4, 0, tileart[25], true);
		//top right
		tiles[13][4][0] = new Tile(13, 4, 0, tileart[26], true);
		//bottom left
		tiles[7][9][0] = new Tile(7, 9, 0, tileart[27], true);
		//bottom right
		tiles[13][9][0] = new Tile(13, 9, 0, tileart[6], true);

		//pad
		tiles[14][6][0] = new TileJumpable(14, 6, 0, tileart[3], false);
		tiles[15][6][0] = new Tile(15, 6, 0, tileart[0], true);
		tiles[15][7][0] = new Tile(15, 7, 0, tileart[2], false);
		tiles[16][6][0] = new TileJumpable(16, 6, 0, tileart[3], false);

		//middle island
		//mid
		for (int i = 18; i < 28; i++) {
			for (int j = 5; j < 18; j++) {
				tiles[i][j][0] = new Tile(i, j, 0, tileart[9], true);
			}
		}
		//top
		for (int i = 18; i < 28; i++) {
			tiles[i][4][0] = new Tile(i, 4, 0, tileart[24], true);
		}
		//left
		for (int i = 5; i < 18; i++) {
			tiles[17][i][0] = new Tile(17, i, 0, tileart[23], true);
		}
		//bottom
		for (int i = 18; i < 28; i++) {
			tiles[i][18][0] = new Tile(i, 18, 0, tileart[8], true);
		}
		//bottom bottom
		for (int i = 17; i < 29; i++) {
			tiles[i][19][0] = new Tile(i, 19, 0, tileart[2], false);
		}
		//right
		for (int i = 5; i < 18; i++) {
			tiles[28][i][0] = new Tile(28, i, 0, tileart[7], true);
		}
		//top left
		tiles[17][4][0] = new Tile(17, 4, 0, tileart[25], true);
		//top right
		tiles[28][4][0] = new Tile(28, 4, 0, tileart[26], true);
		//bottom left
		tiles[17][18][0] = new Tile(17, 18, 0, tileart[27], true);
		//bottom right
		tiles[28][18][0] = new Tile(28, 18, 0, tileart[6], true);

		//pad
		tiles[29][16][0] = new TileJumpable(29, 16, 0, tileart[3], false);
		tiles[30][16][0] = new Tile(30, 16, 0, tileart[0], true);
		tiles[30][17][0] = new Tile(30, 17, 0, tileart[2], false);
		tiles[31][16][0] = new TileJumpable(31, 16, 0, tileart[3], false);

		//right island
		//mid
		for (int i = 33; i < 38; i++) { //5
			for (int j = 14; j < 18; j++) { //4
				tiles[i][j][0] = new Tile(i, j, 0, tileart[9], true);
			}
		}
		//top
		for (int i = 33; i < 38; i++) { //7
			tiles[i][13][0] = new Tile(i, 13, 0, tileart[24], true);
		}
		//left
		for (int i = 14; i < 18; i++) { //
			tiles[32][i][0] = new Tile(32, i, 0, tileart[23], true);
		}
		//bottom
		for (int i = 33; i < 38; i++) {
			tiles[i][18][0] = new Tile(i, 18, 0, tileart[8], true);
		}
		//bottom bottom
		for (int i = 32; i < 39; i++) {
			tiles[i][19][0] = new Tile(i, 19, 0, tileart[2], false);
		}
		//right
		for (int i = 14; i < 18; i++) {
			tiles[38][i][0] = new Tile(38, i, 0, tileart[7], true);
		}
		//top left
		tiles[32][13][0] = new Tile(32, 13, 0, tileart[25], true);
		//top right
		tiles[38][13][0] = new Tile(38, 13, 0, tileart[26], true);
		//bottom left
		tiles[32][18][0] = new Tile(32, 18, 0, tileart[27], true);
		//bottom right
		tiles[38][18][0] = new Tile(38, 18, 0, tileart[6], true);

		//teleportal
		tiles[35][15][0] = new TileExit(35, 15, 0, tileart[22], true, 3, 568, 248, 2);
	}
	else if (level == 3) {
		this->width = 60;
		this->height = 60;
		this->depth = 3;
		this->leftEdge = 0;
		this->topEdge = 0;
		this->rightEdge = this->width * 16;
		this->bottomEdge = this->height * 16;

		for (int i = 0; i < 60; i++) {
			for (int j = 0; j < 60; j++) {
				tiles[i][j][0] = new Tile(i, j, 0, tileart[3], false);
				for (int k = 1; k < 3; k++) {
					tiles[i][j][k] = new Tile(i, j, k, NULL, false);
				}
			}
		}

		//right tower
		//z = 2
		//mid
		tiles[35][16][2] = new Tile(35, 16, 2, tileart[9], true);
		//top
		tiles[35][15][2] = new Tile(35, 15, 2, tileart[24], true);
		//left
		tiles[34][16][2] = new Tile(34, 16, 2, tileart[23], true);
		//bottom
		tiles[35][17][2] = new Tile(35, 17, 2, tileart[8], true);
		//right
		tiles[36][16][2] = new Tile(36, 16, 2, tileart[7], true);
		//top left
		tiles[34][15][2] = new Tile(34, 15, 2, tileart[25], true);
		//top right
		tiles[36][15][2] = new Tile(36, 15, 2, tileart[26], true);
		//bottom left
		tiles[34][17][2] = new Tile(34, 17, 2, tileart[27], true);
		//bottom right
		tiles[36][17][2] = new Tile(36, 17, 2, tileart[6], true);

		//ladder
		tiles[35][18][2] = new TileLadder(35, 18, 2, tileart[11], false);

		//z = 1
		//wall
		for (int i = 34; i < 37; i++) {
			for (int j = 18; j < 22; j++) {
				tiles[i][j][1] = new Tile(i, j, 1, tileart[13], false);
			}
		}
		//ladder
		tiles[35][18][1] = new TileLadder(35, 18, 1, tileart[11], false);
		tiles[35][19][1] = new TileLadder(35, 19, 1, tileart[10], false);
		tiles[35][20][1] = new TileLadder(35, 20, 1, tileart[10], false);
		tiles[35][21][1] = new TileLadder(35, 21, 1, tileart[10], false);
		//above ladder
		tiles[35][17][1] = new Tile(35, 17, 1, NULL, true);
		//below ladder
		tiles[35][22][1] = new Tile(35, 22, 1, NULL, true);

		//z = 0
		//ladder
		tiles[35][21][0] = new TileLadder(35, 21, 0, tileart[10], false);

		//bottom
		for (int i = 33; i < 38; i++) {
			tiles[i][22][0] = new Tile(i, 22, 0, tileart[0], true);
			tiles[i][23][0] = new Tile(i, 23, 0, tileart[2], false);
		}
		//sides
		for (int i = 19; i < 22; i++) {
			//left
			tiles[33][i][0] = new Tile(33, i, 0, tileart[0], true);
			//right
			tiles[37][i][0] = new Tile(37, i, 0, tileart[0], true);
		}

		//left tower
		//z = 2
		//teleportal
		tiles[15][16][2] = new TileExit(15, 16, 2, tileart[22], true, 4, 40, 40, 0);
		//top
		tiles[15][15][2] = new Tile(15, 15, 2, tileart[24], true);
		//left
		tiles[14][16][2] = new Tile(14, 16, 2, tileart[23], true);
		//bottom
		tiles[15][17][2] = new Tile(15, 17, 2, tileart[8], true);
		//right
		tiles[16][16][2] = new Tile(16, 16, 2, tileart[7], true);
		//top left
		tiles[14][15][2] = new Tile(14, 15, 2, tileart[25], true);
		//top right
		tiles[16][15][2] = new Tile(16, 15, 2, tileart[26], true);
		//bottom left
		tiles[14][17][2] = new Tile(14, 17, 2, tileart[27], true);
		//bottom right
		tiles[16][17][2] = new Tile(16, 17, 2, tileart[6], true);

		//ladder
		tiles[15][18][2] = new TileLadder(15, 18, 2, tileart[11], false);

		//z = 1
		//wall
		for (int i = 14; i < 17; i++) {
			for (int j = 18; j < 22; j++) {
				tiles[i][j][1] = new Tile(i, j, 1, tileart[13], false);
			}
		}
		//ladder
		tiles[15][18][1] = new TileLadder(15, 18, 1, tileart[11], false);
		tiles[15][19][1] = new TileLadder(15, 19, 1, tileart[10], false);
		tiles[15][20][1] = new TileLadder(15, 20, 1, tileart[10], false);
		tiles[15][21][1] = new TileLadder(15, 21, 1, tileart[10], false);
		//above ladder
		tiles[15][17][1] = new Tile(15, 17, 1, NULL, true);
		//below ladder
		tiles[15][22][1] = new Tile(15, 22, 1, NULL, true);

		//z = 0
		//ladder
		tiles[15][21][0] = new TileLadder(15, 21, 0, tileart[10], false);

		//bottom
		for (int i = 13; i < 18; i++) {
			tiles[i][22][0] = new Tile(i, 22, 0, tileart[0], true);
			tiles[i][23][0] = new Tile(i, 23, 0, tileart[2], false);
		}

		//sides
		for (int i = 19; i < 22; i++) {
			//left
			tiles[13][i][0] = new Tile(13, i, 0, tileart[0], true);
			//right
			tiles[17][i][0] = new Tile(17, i, 0, tileart[0], true);
		}

		//center z = 0
		for (int i = 19; i < 32; i++) {
			for (int j = 17; j < 25; j++) {
				tiles[i][j][0] = new Tile(i, j, 0, tileart[0], true);
			}
			tiles[i][25][0] = new Tile(i, 25, 0, tileart[2], false);
		}
		for (int i = 17; i < 25; i++) {
			tiles[18][i][0] = new TileJumpable(18, i, 0, tileart[3], false);
			tiles[32][i][0] = new TileJumpable(32, i, 0, tileart[3], false);
		}
		//center left tower
		tiles[20][17][2] = new TileLadder(20, 17, 2, tileart[11], false);
		//ladder
		tiles[20][17][1] = new TileLadder(20, 17, 1, tileart[11], false);
		tiles[20][18][1] = new TileLadder(20, 18, 1, tileart[10], false);
//		tiles[20][18][2] = new Tile(20, 18, 2, tileart[1], true); // test tile for new draw
		tiles[20][19][1] = new TileLadder(20, 19, 1, tileart[10], false);
		tiles[20][17][0] = new Tile(20, 17, 1, NULL, false);
		tiles[20][18][0] = new Tile(20, 18, 1, NULL, false);
		tiles[20][19][0] = new Tile(20, 19, 1, NULL, false);
		//left wall
		tiles[19][17][0] = new Tile(19, 17, 1, tileart[4], false);
		tiles[19][18][0] = new Tile(19, 18, 1, tileart[4], false);
		tiles[19][19][0] = new Tile(19, 19, 1, tileart[4], false);
		//right wall
		tiles[21][17][0] = new Tile(21, 17, 1, tileart[5], false);
		tiles[21][18][0] = new Tile(21, 18, 1, tileart[5], false);
		tiles[21][19][0] = new Tile(21, 19, 1, tileart[5], false);

		tiles[20][19][0] = new TileLadder(20, 19, 0, tileart[10], false);
		//above ladder
		tiles[20][16][1] = new Tile(20, 16, 1, NULL, true);
		//below ladder
		tiles[20][20][1] = new Tile(20, 20, 1, NULL, true);

		//mid
		tiles[20][15][2] = new Tile(20, 15, 2, tileart[9], true);
		//top
		tiles[20][14][2] = new Tile(20, 14, 2, tileart[24], true);
		//left
		tiles[19][15][2] = new Tile(19, 15, 2, tileart[23], true);
		//bottom
		tiles[20][16][2] = new Tile(20, 16, 2, tileart[8], true);
		//right
		tiles[21][15][2] = new Tile(21, 15, 2, tileart[7], true);
		//top left
		tiles[19][14][2] = new Tile(19, 14, 2, tileart[25], true);
		//top right
		tiles[21][14][2] = new Tile(21, 14, 2, tileart[26], true);
		//bottom left
		tiles[19][16][2] = new Tile(19, 16, 2, tileart[27], true);
		//bottom right
		tiles[21][16][2] = new Tile(21, 16, 2, tileart[6], true);

		//pushable area
		tiles[22][17][0] = new Tile(22, 17, 0, tileart[14], true);
		tiles[23][17][0] = new Tile(23, 17, 0, tileart[15], true);
		tiles[24][17][0] = new Tile(24, 17, 0, tileart[16], true);

		tiles[22][18][0] = new Tile(22, 18, 0, tileart[17], true);
		tiles[22][19][0] = new Tile(22, 19, 0, tileart[17], true);
		tiles[22][20][0] = new Tile(22, 20, 0, tileart[17], true);

		tiles[24][18][0] = new Tile(24, 18, 0, tileart[21], true);
		tiles[24][19][0] = new Tile(24, 19, 0, tileart[21], true);
		tiles[24][20][0] = new Tile(24, 20, 0, tileart[21], true);

		tiles[22][21][0] = new Tile(22, 21, 0, tileart[18], true);
		tiles[23][21][0] = new Tile(23, 21, 0, tileart[19], true);
		tiles[24][21][0] = new Tile(24, 21, 0, tileart[20], true);

		tiles[23][18][0] = new TilePushable(23, 18, 0, tileart[0], true);
		tiles[23][19][0] = new TilePushable(23, 19, 0, tileart[0], true);
		tiles[23][20][0] = new TilePushable(23, 20, 0, tileart[0], true);

		//jumpable
		for (int i = 22; i < 25; i++) {
			for (int j = 13; j < 22; j++) {
				tiles[i][j][2] = new TileJumpable(i, j, 2, NULL, false);
			}
		}
		//center right tower
		//center wall
		tiles[26][17][0] = new Tile(26, 17, 1, tileart[5], false);
		tiles[26][18][0] = new Tile(26, 18, 1, tileart[5], false);
		tiles[26][19][0] = new Tile(26, 19, 1, tileart[5], false);
		//left wall
		tiles[25][17][0] = new Tile(25, 17, 1, tileart[4], false);
		tiles[25][18][0] = new Tile(25, 18, 1, tileart[4], false);
		tiles[25][19][0] = new Tile(25, 19, 1, tileart[4], false);
		//right wall
		tiles[27][17][0] = new Tile(27, 17, 1, tileart[4], false);
		tiles[27][18][0] = new Tile(27, 18, 1, tileart[4], false);
		tiles[27][19][0] = new Tile(27, 19, 1, tileart[4], false);

		//mid
		tiles[26][15][2] = new Tile(26, 15, 2, tileart[9], true);
		//top
		tiles[26][14][2] = new Tile(26, 14, 2, tileart[24], true);
		//left
		tiles[25][15][2] = new Tile(25, 15, 2, tileart[23], true);
		//bottom
		tiles[26][16][2] = new Tile(26, 16, 2, tileart[8], true);
		//right
		tiles[27][15][2] = new Tile(27, 15, 2, tileart[7], true);
		//top left
		tiles[25][14][2] = new Tile(25, 14, 2, tileart[25], true);
		//top right
		tiles[27][14][2] = new Tile(27, 14, 2, tileart[26], true);
		//bottom left
		tiles[25][16][2] = new Tile(25, 16, 2, tileart[27], true);
		//bottom right
		tiles[27][16][2] = new Tile(27, 16, 2, tileart[6], true);
	}
}

int Map::getDepth() {
	return depth;
}

int Map::getLevel() {
	return level;
}

//istream& operator >> (istream& in, Map& map) {
//	in >> map.depth;
//	in >> map.width;
//	in >> map.height;
//	for (int i < )
//	return in;
//}
//
//ostream& operator << (ostream& out, Map& map) {
//	out << map.depth;
//	return out;
//}
