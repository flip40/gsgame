#include<SDL.h>
#undef main
#include<stdio.h>
#include<glew.h>
#include<string>
#include<algorithm>
#include<cmath>
#include<iostream>
#include"DrawUtils.h"
#include"Drawable.h"
#include"Camera.h"
#include"Character.h"
#include"Map.h"
#include"Animation.h"
#include"AnimationJump.h"
#include"AnimationClimbUp.h"
#include"AnimationClimbDown.h"
#include"Frame.h"
#include"AI.h"
#include<iostream>
#include<fstream>
#include"TileJumpable.h"
#include"TilePushable.h"
#include"Column.h"

using namespace std;

char shouldExit = 0;
bool playing = false;
bool ending = false;
int menuState = 0;
float charPrevX = 0;
float charPrevY = 0;
float upHeld = 0;
float downHeld = 0;
float leftHeld = 0;
float rightHeld = 0;

int coins = 0;

/* The previous frame's keyboard state */
unsigned char kbPrevState[SDL_NUM_SCANCODES] = { 0 };
/* The current frame's keyboard state */
const unsigned char* kbState = NULL;

/* Map and characters */
Camera* camera;
Map* map;
Character* character;
Character* jenna;
AI* jennaAI;
queue<AI*> aiQueue;
vector<Character*> gold;
vector<AI*> goldAI;
int lastLevel = 0;
Column* column = NULL;

/* Physics runs at 100fps, or 10ms / physics frame */
float physicsDeltaTime = 1 / 100.0f;
int physicsDeltaMs = 10;
Uint32 lastPhysicsFrameMs;

int main(void)
{
	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return 1;
	}
	/* Create the window, OpenGL context */
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_Window* window = SDL_CreateWindow("GoldenSun 2 Race", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 480, 320, SDL_WINDOW_OPENGL);
	if (!window) {
		fprintf(stderr, "Could not create window.ErrorCode = %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	SDL_GL_CreateContext(window);

	/* Make sure we have a recent version of OpenGL */
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		fprintf(stderr, "Colud not initialize glew.ErrorCode = %s\n", glewGetErrorString(glewError));
		SDL_Quit();
		return 1;
	}
	//if (!GLEW_VERSION_3_0) {
	if (!GLEW_VERSION_2_0) {
		fprintf(stderr, "OpenGL max supported version is too low.\n");
		SDL_Quit();
		return 1;
	}

	/* Setup OpenGL state */
	glViewport(0, 0, 480, 320);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, 240, 160, 0, 0, 100);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Keep a pointer to SDL's internal keyboard state */
	kbState = SDL_GetKeyboardState(NULL);

	/* load the textures */
	//menu
	GLuint menuBg = glTexImageTGAFile("resources/menu/menubg.tga", NULL, NULL);
	GLuint menuArrow = glTexImageTGAFile("resources/menu/arrow.tga", NULL, NULL);
	GLuint gameOverBg = glTexImageTGAFile("resources/menu/gameover.tga", NULL, NULL);

	GLuint numbers[10] = {
		glTexImageTGAFile("resources/nums/0.tga", NULL, NULL),
		glTexImageTGAFile("resources/nums/1.tga", NULL, NULL),
		glTexImageTGAFile("resources/nums/2.tga", NULL, NULL),
		glTexImageTGAFile("resources/nums/3.tga", NULL, NULL),
		glTexImageTGAFile("resources/nums/4.tga", NULL, NULL),
		glTexImageTGAFile("resources/nums/5.tga", NULL, NULL),
		glTexImageTGAFile("resources/nums/6.tga", NULL, NULL),
		glTexImageTGAFile("resources/nums/7.tga", NULL, NULL),
		glTexImageTGAFile("resources/nums/8.tga", NULL, NULL),
		glTexImageTGAFile("resources/nums/9.tga", NULL, NULL),
	};

	GLuint scoreboard = glTexImageTGAFile("resources/scoreboard.tga", NULL, NULL);

	camera = new Camera(240, 160); //creates camera object
	vector<Frame> cStanding = {
		Frame(glTexImageTGAFile("resources/chars/felix/standing/standing1.tga", NULL, NULL), 22, 38),
		Frame(glTexImageTGAFile("resources/chars/felix/standing/standing2.tga", NULL, NULL), 22, 35),
		Frame(glTexImageTGAFile("resources/chars/felix/standing/standing3.tga", NULL, NULL), 20, 39),
		Frame(glTexImageTGAFile("resources/chars/felix/standing/standing4.tga", NULL, NULL), 20, 39),
		Frame(glTexImageTGAFile("resources/chars/felix/standing/standing5.tga", NULL, NULL), 20, 40),
		Frame(glTexImageTGAFile("resources/chars/felix/standing/standing6.tga", NULL, NULL), 20, 40),
		Frame(glTexImageTGAFile("resources/chars/felix/standing/standing7.tga", NULL, NULL), 21, 39),
		Frame(glTexImageTGAFile("resources/chars/felix/standing/standing8.tga", NULL, NULL), 21, 39),
	};
	vector<Frame> cRunUp = {
		Frame(glTexImageTGAFile("resources/chars/felix/running/up1.tga", NULL, NULL), 23, 37),
		Frame(glTexImageTGAFile("resources/chars/felix/running/up2.tga", NULL, NULL), 21, 37),
		Frame(glTexImageTGAFile("resources/chars/felix/running/up3.tga", NULL, NULL), 21, 37),
		Frame(glTexImageTGAFile("resources/chars/felix/running/up4.tga", NULL, NULL), 23, 37),
		Frame(glTexImageTGAFile("resources/chars/felix/running/up5.tga", NULL, NULL), 26, 37),
		Frame(glTexImageTGAFile("resources/chars/felix/running/up6.tga", NULL, NULL), 24, 37),
	};
	vector<Frame> cRunDown = {
		Frame(glTexImageTGAFile("resources/chars/felix/running/down1.tga", NULL, NULL), 20, 35),
		Frame(glTexImageTGAFile("resources/chars/felix/running/down2.tga", NULL, NULL), 21, 35),
		Frame(glTexImageTGAFile("resources/chars/felix/running/down3.tga", NULL, NULL), 23, 35),
		Frame(glTexImageTGAFile("resources/chars/felix/running/down4.tga", NULL, NULL), 23, 35),
		Frame(glTexImageTGAFile("resources/chars/felix/running/down5.tga", NULL, NULL), 23, 35),
		Frame(glTexImageTGAFile("resources/chars/felix/running/down6.tga", NULL, NULL), 23, 35),
	};
	vector<Frame> cRunLeft = {
		Frame(glTexImageTGAFile("resources/chars/felix/running/left1.tga", NULL, NULL), 26, 36),
		Frame(glTexImageTGAFile("resources/chars/felix/running/left2.tga", NULL, NULL), 29, 36),
		Frame(glTexImageTGAFile("resources/chars/felix/running/left3.tga", NULL, NULL), 29, 36),
		Frame(glTexImageTGAFile("resources/chars/felix/running/left4.tga", NULL, NULL), 25, 36),
		Frame(glTexImageTGAFile("resources/chars/felix/running/left5.tga", NULL, NULL), 28, 36),
		Frame(glTexImageTGAFile("resources/chars/felix/running/left6.tga", NULL, NULL), 29, 36),
	};
	vector<Frame> cRunRight = {
		Frame(glTexImageTGAFile("resources/chars/felix/running/right1.tga", NULL, NULL), 26, 37),
		Frame(glTexImageTGAFile("resources/chars/felix/running/right2.tga", NULL, NULL), 29, 37),
		Frame(glTexImageTGAFile("resources/chars/felix/running/right3.tga", NULL, NULL), 29, 37),
		Frame(glTexImageTGAFile("resources/chars/felix/running/right4.tga", NULL, NULL), 25, 37),
		Frame(glTexImageTGAFile("resources/chars/felix/running/right5.tga", NULL, NULL), 28, 37),
		Frame(glTexImageTGAFile("resources/chars/felix/running/right6.tga", NULL, NULL), 29, 37),
	};
	vector<Frame> cRunUpLeft = {
		Frame(glTexImageTGAFile("resources/chars/felix/running/upleft1.tga", NULL, NULL), 25, 36),
		Frame(glTexImageTGAFile("resources/chars/felix/running/upleft2.tga", NULL, NULL), 26, 36),
		Frame(glTexImageTGAFile("resources/chars/felix/running/upleft3.tga", NULL, NULL), 25, 36),
		Frame(glTexImageTGAFile("resources/chars/felix/running/upleft4.tga", NULL, NULL), 22, 36),
		Frame(glTexImageTGAFile("resources/chars/felix/running/upleft5.tga", NULL, NULL), 24, 36),
		Frame(glTexImageTGAFile("resources/chars/felix/running/upleft6.tga", NULL, NULL), 24, 36),
	};
	vector<Frame> cRunUpRight = {
		Frame(glTexImageTGAFile("resources/chars/felix/running/upright1.tga", NULL, NULL), 24, 36),
		Frame(glTexImageTGAFile("resources/chars/felix/running/upright2.tga", NULL, NULL), 24, 36),
		Frame(glTexImageTGAFile("resources/chars/felix/running/upright3.tga", NULL, NULL), 22, 36),
		Frame(glTexImageTGAFile("resources/chars/felix/running/upright4.tga", NULL, NULL), 25, 36),
		Frame(glTexImageTGAFile("resources/chars/felix/running/upright5.tga", NULL, NULL), 26, 36),
		Frame(glTexImageTGAFile("resources/chars/felix/running/upright6.tga", NULL, NULL), 25, 36),
	};
	vector<Frame> cRunDownLeft = {
		Frame(glTexImageTGAFile("resources/chars/felix/running/downleft1.tga", NULL, NULL), 24, 35),
		Frame(glTexImageTGAFile("resources/chars/felix/running/downleft2.tga", NULL, NULL), 26, 35),
		Frame(glTexImageTGAFile("resources/chars/felix/running/downleft3.tga", NULL, NULL), 25, 35),
		Frame(glTexImageTGAFile("resources/chars/felix/running/downleft4.tga", NULL, NULL), 25, 35),
		Frame(glTexImageTGAFile("resources/chars/felix/running/downleft5.tga", NULL, NULL), 26, 35),
		Frame(glTexImageTGAFile("resources/chars/felix/running/downleft6.tga", NULL, NULL), 26, 35),
	};
	vector<Frame> cRunDownRight = {
		Frame(glTexImageTGAFile("resources/chars/felix/running/downright1.tga", NULL, NULL), 24, 35),
		Frame(glTexImageTGAFile("resources/chars/felix/running/downright2.tga", NULL, NULL), 26, 35),
		Frame(glTexImageTGAFile("resources/chars/felix/running/downright3.tga", NULL, NULL), 25, 35),
		Frame(glTexImageTGAFile("resources/chars/felix/running/downright4.tga", NULL, NULL), 25, 35),
		Frame(glTexImageTGAFile("resources/chars/felix/running/downright5.tga", NULL, NULL), 26, 35),
		Frame(glTexImageTGAFile("resources/chars/felix/running/downright6.tga", NULL, NULL), 26, 35),
	};
	vector<Frame> cJumpUp = {
		Frame(glTexImageTGAFile("resources/chars/felix/jumping/up1.tga", NULL, NULL), 22, 33),
		Frame(glTexImageTGAFile("resources/chars/felix/jumping/up2.tga", NULL, NULL), 22, 33),
		Frame(glTexImageTGAFile("resources/chars/felix/jumping/up3.tga", NULL, NULL), 22, 33),
	};
	vector<Frame> cJumpDown = {
		Frame(glTexImageTGAFile("resources/chars/felix/jumping/down1.tga", NULL, NULL), 22, 32),
		Frame(glTexImageTGAFile("resources/chars/felix/jumping/down2.tga", NULL, NULL), 22, 32),
		Frame(glTexImageTGAFile("resources/chars/felix/jumping/down3.tga", NULL, NULL), 22, 32),
	};
	vector<Frame> cJumpLeft = {
		Frame(glTexImageTGAFile("resources/chars/felix/jumping/left1.tga", NULL, NULL), 23, 34),
		Frame(glTexImageTGAFile("resources/chars/felix/jumping/left2.tga", NULL, NULL), 23, 34),
		Frame(glTexImageTGAFile("resources/chars/felix/jumping/left3.tga", NULL, NULL), 23, 34),
	};
	vector<Frame> cJumpRight = {
		Frame(glTexImageTGAFile("resources/chars/felix/jumping/right1.tga", NULL, NULL), 23, 34),
		Frame(glTexImageTGAFile("resources/chars/felix/jumping/right2.tga", NULL, NULL), 23, 34),
		Frame(glTexImageTGAFile("resources/chars/felix/jumping/right3.tga", NULL, NULL), 23, 34),
	};
	vector<Frame> cClimbDown = {
		Frame(glTexImageTGAFile("resources/chars/felix/climbing/down1.tga", NULL, NULL), 20, 29),
		Frame(glTexImageTGAFile("resources/chars/felix/climbing/down2.tga", NULL, NULL), 20, 29),
		Frame(glTexImageTGAFile("resources/chars/felix/climbing/down3.tga", NULL, NULL), 20, 29),
	};
	vector<Frame> cClimb = {
		Frame(glTexImageTGAFile("resources/chars/felix/climbing/climbing1.tga", NULL, NULL), 19, 27),
		Frame(glTexImageTGAFile("resources/chars/felix/climbing/climbing2.tga", NULL, NULL), 19, 27),
		Frame(glTexImageTGAFile("resources/chars/felix/climbing/climbing3.tga", NULL, NULL), 19, 27),
		Frame(glTexImageTGAFile("resources/chars/felix/climbing/climbing4.tga", NULL, NULL), 19, 27),
	};
	vector<Frame> cClimbUp = {
		Frame(glTexImageTGAFile("resources/chars/felix/climbing/up1.tga", NULL, NULL), 16, 31),
		Frame(glTexImageTGAFile("resources/chars/felix/climbing/up2.tga", NULL, NULL), 16, 31),
		Frame(glTexImageTGAFile("resources/chars/felix/climbing/up3.tga", NULL, NULL), 16, 31),
		Frame(glTexImageTGAFile("resources/chars/felix/climbing/up4.tga", NULL, NULL), 16, 31),
	};
	vector<Animation*> charAnims = {
		new Animation(-1, cStanding),
		new Animation(.1, cRunUp),
		new Animation(.1, cRunDown),
		new Animation(.1, cRunLeft),
		new Animation(.1, cRunRight),
		new Animation(.1, cRunUpLeft),
		new Animation(.1, cRunUpRight),
		new Animation(.1, cRunDownLeft),
		new Animation(.1, cRunDownRight),
		new AnimationJump(.5, cJumpUp),
		new AnimationJump(.5, cJumpDown),
		new AnimationJump(.5, cJumpLeft),
		new AnimationJump(.5, cJumpRight),
		new AnimationClimbDown(.3, cClimbDown),
		new Animation(.1, cClimb),
		new AnimationClimbUp(.4, cClimbUp) };
//	character = new Character(40, 40, 0, 16, 31, 8, 32, charAnims);
//	character = new Character(400, 350, 0, 16, 31, 8, 32, charAnims);
	character = new Character(400, 350, 0, 16, 31, 8, 32, charAnims, true); //map 3
	vector<Frame> jStanding = {
		Frame(glTexImageTGAFile("resources/chars/jenna/standing/standing2.tga", NULL, NULL), 26, 33), //hacky atm, have a change in character that will be removed later
		Frame(glTexImageTGAFile("resources/chars/jenna/standing/standing2.tga", NULL, NULL), 26, 33), //assumes 2 is down, as in the future all characters should have all 8 directions
	};
//	vector<Animation*> jennaAnims = {
//		new Animation(-1, jStanding),
//	};
	vector<Animation*> jennaAnims = {
		new Animation(-1, cStanding),
		new Animation(.1, cRunUp),
		new Animation(.1, cRunDown),
		new Animation(.1, cRunLeft),
		new Animation(.1, cRunRight),
		new Animation(.1, cRunUpLeft),
		new Animation(.1, cRunUpRight),
		new Animation(.1, cRunDownLeft),
		new Animation(.1, cRunDownRight),
		new AnimationJump(.5, cJumpUp),
		new AnimationJump(.5, cJumpDown),
		new AnimationJump(.5, cJumpLeft),
		new AnimationJump(.5, cJumpRight),
		new AnimationClimbDown(.3, cClimbDown),
		new Animation(.1, cClimb),
		new AnimationClimbUp(.4, cClimbUp)
	};

	map = new Map(3);
 
//	jenna = new Character(200, 90, 0, 26, 33, 32, jennaAnims);
//	jenna = new Character(350.3, 350.3, 0, 16, 31, 8, 32, jennaAnims, false);
	jenna = new Character(350.3, 350.3, 0, 16, 31, 8, 32, jennaAnims, true);
	jennaAI = new AI(map, jenna, character, &aiQueue);

	vector<Frame> goldFrames = {
		Frame(glTexImageTGAFile("resources/gold.tga", NULL, NULL), 8, 10), //hacky atm, have a change in character that will be removed later
		Frame(glTexImageTGAFile("resources/gold.tga", NULL, NULL), 8, 10), //assumes 2 is down, as in the future all characters should have all 8 directions
	};
//	vector<Animation*> goldAnims = { new Animation(-1, goldFrames) };
	vector<Animation*> goldAnims = {
		new Animation(-1, goldFrames),
		new Animation(-1, goldFrames),
		new Animation(-1, goldFrames),
		new Animation(-1, goldFrames),
		new Animation(-1, goldFrames),
		new Animation(-1, goldFrames),
		new Animation(-1, goldFrames),
		new Animation(-1, goldFrames),
		new Animation(-1, goldFrames)
	};

	//Column* column = new Column(64, 80, 0, 18, 50, glTexImageTGAFile("resources/maps/column.tga", NULL, NULL));

	//((TilePushable*)map->getTile(4, 5, 0))->setColumn(column);

	vector<Character*> characters = { character, jenna };
	vector<Drawable*> drawables = { (Drawable*)character, (Drawable*)jenna/*, (Drawable*)column*/ };
	vector<AI*> ai = { jennaAI };

	Uint32 lastFrameMs;
	Uint32 currentFrameMs = SDL_GetTicks();
	
	/* The game loop */
	while (!shouldExit) {
		/* kbState is updated by the message pump. Copy over the old state before the pump! */
		std::memcpy(kbPrevState, kbState, sizeof(kbPrevState));

		/* currentFrameMs is updated after pump. Copy over the old state before this! */
		lastFrameMs = currentFrameMs;

		// Handle OS message pump
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					shouldExit = 1;
			}
		}
		currentFrameMs = SDL_GetTicks();
		float deltaTime = (currentFrameMs - lastFrameMs) / 1000.0f;

		/* Game logic goes here */
		if (!playing && !ending) {
			/* Interpret keyboard input */
			bool up = false;
			bool down = false;
			if (kbState[SDL_SCANCODE_UP] || kbState[SDL_SCANCODE_W]) {
				up = true;
			}
			if (kbState[SDL_SCANCODE_DOWN] || kbState[SDL_SCANCODE_S]) {
				down = true;
			}
			if (kbState[SDL_SCANCODE_RETURN]) {
				if (!kbPrevState[SDL_SCANCODE_RETURN]) {
					if (menuState == 0) {
						playing = true;
						lastPhysicsFrameMs = currentFrameMs;
						continue;
					}
					else if (menuState == 1) {
						SDL_Quit();
						return 0;
					}
				}
			}

			if (up) {
				menuState = 0;
			}
			else if (down) {
				menuState = 1;
			}

			/* Clear screen */
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			/* Draw sprites */
			glDrawSprite(menuBg, 0, 0, 240, 160);
			if (menuState == 0) {
				glDrawSprite(menuArrow, 70, 59, 20, 20);
			}
			else if (menuState == 1) {
				glDrawSprite(menuArrow, 70, 92, 20, 20);
			}
		}
		else if (ending) {
			/* Interpret keyboard input */
			bool left = false;
			bool right = false;
			if (kbState[SDL_SCANCODE_LEFT] || kbState[SDL_SCANCODE_A]) {
				left = true;
			}
			if (kbState[SDL_SCANCODE_RIGHT] || kbState[SDL_SCANCODE_D]) {
				right = true;
			}
			if (kbState[SDL_SCANCODE_RETURN]) {
				if (menuState == 0) {
					ending = false;
					coins = 0;
					continue;
				}
				else if (menuState == 1) {
					SDL_Quit();
					return 0;
				}
			}

			if (left) {
				menuState = 0;
			}
			else if (right) {
				menuState = 1;
			}

			/* Clear screen */
			glClearColor(0, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			/* Draw sprites */
			glDrawSprite(gameOverBg, 0, 0, 240, 160);
			if (menuState == 0) {
				glDrawSprite(menuArrow, 15, 127, 20, 20);
			}
			else if (menuState == 1) {
				glDrawSprite(menuArrow, 130, 127, 20, 20);
			}

			//draw score
			int score = coins;
			int count = 0;
			while (score > 9) {
				glDrawSprite(numbers[score % 10], 115 - (12 * count), 59, 11, 15);
				score /= 10;
				count++;
			}
			glDrawSprite(numbers[score], 115 - (12 * count), 59, 11, 15);
		}
		else {
			/* Interpret keyboard input */
			bool up = false;
			bool down = false;
			bool left = false;
			bool right = false;
			bool mult = false;
			if (kbState[SDL_SCANCODE_LSHIFT] || kbState[SDL_SCANCODE_RSHIFT]) {
				mult = true;
			}
			if (kbState[SDL_SCANCODE_LEFT] || kbState[SDL_SCANCODE_A]) {
				left = true;
			}
			if (kbState[SDL_SCANCODE_RIGHT] || kbState[SDL_SCANCODE_D]) {
				right = true;
			}
			if (kbState[SDL_SCANCODE_UP] || kbState[SDL_SCANCODE_W]) {
				up = true;
			}
			if (kbState[SDL_SCANCODE_DOWN] || kbState[SDL_SCANCODE_S]) {
				down = true;
			}

			// check for "gesture". Also check if only in cardinal directions (only works if trying to go just left, just right, etc.)
			if (charPrevX == character->getX() && charPrevY == character->getY() && (up + down + left + right) == 1) {
				if (up) upHeld += deltaTime;
				else upHeld = 0;
				if (down) downHeld += deltaTime;
				else downHeld = 0;
				if (left) leftHeld += deltaTime;
				else leftHeld = 0;
				if (right) rightHeld += deltaTime;
				else rightHeld = 0;
			}
			else {
				upHeld = 0;
				downHeld = 0;
				leftHeld = 0;
				rightHeld = 0;
			}
			charPrevX = character->getX();
			charPrevY = character->getY();

			if (upHeld > .25f || downHeld > .25f || leftHeld > .25f || rightHeld > .25f) {
				float x = ((int)character->getX() / 16) + (right - left);
				float y = 0;
				if (character->isClimbing() && !(upHeld && downHeld)) {
					if (upHeld) {
						y = ((int)character->getY() / 16) - 2;
					}
					else if (downHeld) {
						y = ((int)character->getY() / 16) + 1;
					}
				}
				else {
					y = ((int)character->getY() / 16) + (down - up);
				}
				//cout << "cx: " << ((int)character->getX() / 16) << " cy: " << ((int)character->getY() / 16) << "\n";
				//cout << "mx: " << (left - right) << " my: " << (up - down) << "\n";
				//cout << "x: " << x << " y: " << y << "\n";
				if (x >= 0 && x <= (map->getWidth() / 16) && y >= 0 && (y <= map->getHeight() / 16)) {
					//cout << "yes!" << "\n";
					map->getTile(x, y, character->getZ())->animate(character, map, deltaTime);
				}
			}

			//cout << "px: " << charPrevX << " py: " << charPrevY << "\n";
			//cout << "cx: " << character->getX() << " cy: " << character->getY() << "\n";
			//cout << "up: " << upHeld << " down: " << downHeld << " left: " << leftHeld << " right: " << rightHeld << "\n";

			/* AI update */
			//std::cout << aiQueue.size() << "\n";

			for (int i = 0; i < aiQueue.size(); i++) {
				if (SDL_GetTicks() - currentFrameMs < 10) {
					aiQueue.front()->decide();
				}
				else break;
			}
			
			Tile* tile = map->getTile(character->getX() / 16, character->getY() / 16, character->getZ());
			if (tile) tile->walkedOn(character, map, deltaTime);

			/* Physics update */
//			int physcount = 0;
			Uint32 deltaMs = (currentFrameMs - lastFrameMs);
			physicsDeltaMs = 10;
			if (deltaMs < physicsDeltaMs) physicsDeltaMs = currentFrameMs - lastFrameMs;
			while (physicsDeltaMs != 0 && lastPhysicsFrameMs + physicsDeltaMs <= currentFrameMs) {
				// 1. react normally, 2. revert those that failed, 3. revert those that did not fail originally, but may now be failing 4. ??? 5. profit! (wip)
				/* 1. Physics movement */
				character->move(up, down, left, right, mult, physicsDeltaMs / 1000.0f, map);
				if (column) column->move((float)physicsDeltaMs / 1000.0f);
				jennaAI->execute(physicsDeltaMs / 1000.0f);
				for (int i = 0; i < goldAI.size(); i++) goldAI[i]->execute(physicsDeltaMs / 1000.0f);
				/* 2. Physics collision detection */
				//check against other characters (a bit hardcoded at the moment, need to think a bit more. Want to avoid unnecessary repeat checks)
				for (int i = 0; i < gold.size(); i++) {
					float distance = sqrt(pow((character->getX() - gold[i]->getX()), 2) + (pow((character->getY() - gold[i]->getY()), 2)));
					float radius = character->getRadius() + gold[i]->getRadius();
					if (sqrt(pow((character->getX() - gold[i]->getX()), 2) + (pow((character->getY() - gold[i]->getY()), 2))) < (character->getRadius() + gold[i]->getRadius())) {
						gold.erase(gold.begin() + i);
						goldAI.erase(goldAI.begin() + i);
						coins++;
					}
				}
				if (sqrt(pow((character->getX() - jenna->getX()), 2) + (pow((character->getY() - jenna->getY()), 2))) < (character->getRadius() + jenna->getRadius())) {
					//Improvements! Just need to abstract this off somewhere and use it for character collisions, which covers about 99% of collision in this game style
					//Just need to set up an interpretation of when/how this should be applied to non-circular objects, but I think I have a plan for that...
					//vx/y is character movement vector
					float vx = character->getX() - character->getPrevX();
					float vy = character->getY() - character->getPrevY();
					//nx/y is the normal of a wall directly between felix and jenna, used for "bouncing" in the right direction. Points to felix (collider/mover)
					//this can probably just be resolved for both characters with no harm, I think. Should test it though.
					float nx = character->getX() - jenna->getX();
					float ny = character->getY() - jenna->getY();
					//length of n, used to normalize nx/y
					float lenn = sqrt(nx*nx + ny*ny);
					nx = nx / lenn;
					ny = ny / lenn;
					//v dot n, or dot product of v * n
					float vdotn = vx*nx + vy*ny;
					//new vx/y (new character velocity) based on the above. Because this is the bounce vector (from wall out), this is changed below.
					float nvx = -2 * vdotn * nx + vx;
					float nvy = -2 * vdotn * ny + vy;
					//changes the bounce vx/vy into a direct path to the bounce location. So instead of bouncing, we just "roll" as you would expect if you pushed
					//a circle against another circle. Or any shape really.
					float finalvx = (nvx - vx);
					float finalvy = (nvy - vy);
					//set new character position
					character->setPos(character->getX() + finalvx, character->getY() + finalvy, map);
					//still push Jenna back because it's fun ^.^
					jenna->setPos(jenna->getX() - finalvx, jenna->getY() - finalvy, map);
				}

				/* 3. Physics collision resolution */
				lastPhysicsFrameMs += physicsDeltaMs;
//				physcount++;
			}
//			cout << lastPhysicsFrameMs << "\n";
//			cout << lastFrameMs << "\n";
//			cout << currentFrameMs << "\n";
//			cout << physicsDeltaMs << "\n";

			/* Update non physics values */
			character->updateAnimation(up, down, left, right, deltaTime);
			camera->centerOnPC(character, map);

			/* Clear screen */
			glClearColor(0, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			/* Draw sprites */
//			map->draw(camera);
			//sort characters by Y coordinates to preserve draw order (above is behind, so lower Y's get pushed to the front and thus drawn first)
			//sort(characters.begin(), characters.end(), sortCharByY);
			//for (int i = 0; i < characters.size(); i++) {
			//	characters[i]->draw(camera);
			//}
//			for (int i = 0; i < drawables.size(); i++) {
//				drawables[i]->draw(camera);
//			}

			sort(drawables.begin(), drawables.end(), sortByY);

			camera->draw(map, drawables);
			
			for (int i = 0; i < gold.size(); i++) {
				gold[i]->draw(camera);
			}
			
			//draw score
			int score = coins;
			int count = 0;
			glDrawSprite(scoreboard, 225, 0, 15, 10);
			while (score > 9) {
				glDrawSprite(numbers[score % 10], 235 - (6 * count), 1, 5, 7);
				score /= 10;
				count++;
			}
			glDrawSprite(numbers[score], 235 - (6 * count), 1, 5, 7);

			//check for level change
			if (lastLevel != map->getLevel()) {
				lastLevel = map->getLevel();
				while (gold.size() > 0) {
					gold.erase(gold.begin());
					goldAI.erase(goldAI.begin());
				}
				if (map->getLevel() == 1) {
					//top left
					gold.push_back(new Character(64, 64, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[0], character, &aiQueue));
					gold.push_back(new Character(20, 20, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[1], character, &aiQueue));

					//top right
					gold.push_back(new Character(272, 64, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[2], character, &aiQueue));
					gold.push_back(new Character(292, 20, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[3], character, &aiQueue));

					//bottom left
					gold.push_back(new Character(64, 272, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[4], character, &aiQueue));
					gold.push_back(new Character(20, 292, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[5], character, &aiQueue));

					//bottom right
					gold.push_back(new Character(272, 272, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[6], character, &aiQueue));
					gold.push_back(new Character(292, 292, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[7], character, &aiQueue));

					//center left
					gold.push_back(new Character(104, 120, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[8], character, &aiQueue));
					gold.push_back(new Character(104, 180, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[9], character, &aiQueue));

					//center right
					gold.push_back(new Character(232, 120, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[10], character, &aiQueue));
					gold.push_back(new Character(232, 180, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[11], character, &aiQueue));

				}
				else if (map->getLevel() == 2) {
					//center left
					gold.push_back(new Character(304, 120, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[0], character, &aiQueue));
					gold.push_back(new Character(304, 140, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[1], character, &aiQueue));
					gold.push_back(new Character(304, 160, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[2], character, &aiQueue));
					gold.push_back(new Character(304, 180, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[3], character, &aiQueue));
					gold.push_back(new Character(304, 200, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[4], character, &aiQueue));
					gold.push_back(new Character(304, 220, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[5], character, &aiQueue));
					gold.push_back(new Character(304, 240, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[6], character, &aiQueue));

					//center right
					gold.push_back(new Character(432, 120, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[7], character, &aiQueue));
					gold.push_back(new Character(432, 140, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[8], character, &aiQueue));
					gold.push_back(new Character(432, 160, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[9], character, &aiQueue));
					gold.push_back(new Character(432, 180, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[10], character, &aiQueue));
					gold.push_back(new Character(432, 200, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[11], character, &aiQueue));
					gold.push_back(new Character(432, 220, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[12], character, &aiQueue));
					gold.push_back(new Character(432, 240, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[13], character, &aiQueue));
				}
				else if (map->getLevel() == 3) {
					//center
					gold.push_back(new Character(370, 324, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[0], character, &aiQueue));
					gold.push_back(new Character(390, 324, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[1], character, &aiQueue));
					gold.push_back(new Character(410, 324, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[2], character, &aiQueue));
					gold.push_back(new Character(430, 324, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[3], character, &aiQueue));
					gold.push_back(new Character(450, 324, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[4], character, &aiQueue));
					gold.push_back(new Character(470, 324, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[5], character, &aiQueue));
					gold.push_back(new Character(490, 324, 0, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[6], character, &aiQueue));


					gold.push_back(new Character(420, 245, 2, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[7], character, &aiQueue));
					gold.push_back(new Character(416, 240, 2, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[8], character, &aiQueue));
					gold.push_back(new Character(410, 235, 2, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[9], character, &aiQueue));
					gold.push_back(new Character(425, 250, 2, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[10], character, &aiQueue));
					gold.push_back(new Character(405, 230, 2, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[11], character, &aiQueue));

					gold.push_back(new Character(340, 230, 2, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[12], character, &aiQueue));
					gold.push_back(new Character(320, 240, 2, 8, 10, 8, 16, goldAnims, false));
					goldAI.push_back(new AI(map, gold[13], character, &aiQueue));

					column = new Column(368, 320, 0, 18, 50, glTexImageTGAFile("resources/maps/column.tga", NULL, NULL));
					((TilePushable*)map->getTile(23, 20, 0))->setColumn(column);
					drawables.push_back(column);
				}
			}

			//check for end game
			if (map->getLevel() == 4) {
				playing = false;
				ending = true;
				map->setLevel(1);
				camera->setPos(0, 0);
				drawables.erase(drawables.begin() + 1);
			}
		}

		/* Swap window */
		SDL_GL_SwapWindow(window);

		/* FPS counter */
		//std::cout << (1/deltaTime) << " " << "1/" << deltaTime << "\n";
	}
//	ofstream mapfile;
//	mapfile.open("/Users/jludeman/Desktop/test.map");
//	mapfile << map;
//	mapfile.close();
	SDL_Quit();
	return 0;
}
