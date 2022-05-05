#include "Game.h"
#include "Frame.h"
#include "DrawUtils.h"
#include "Animation.h"
#include "AnimationJump.h"
#include "AnimationClimbDown.h"
#include "AnimationClimbUp.h"

Game::Game(string saveFile) {
	//if saveFile, load characters, else generate new gamestate
	
	//new gamestate
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
	character = new Character(400, 350, 0, 16, 31, 8, 32, charAnims);
	vector<Frame> jStanding = {
		Frame(glTexImageTGAFile("resources/chars/jenna/standing/standing2.tga", NULL, NULL), 26, 33), //hacky atm, have a change in character that will be removed later
		Frame(glTexImageTGAFile("resources/chars/jenna/standing/standing2.tga", NULL, NULL), 26, 33), //assumes 2 is down, as in the future all characters should have all 8 directions
	};
	vector<Animation*> jennaAnims = {
		new Animation(-1, jStanding),
	};
	
	map = new Map(3);
	
	Character* jenna = new Character(350.3, 350.3, 0, 16, 31, 8, 32, jennaAnims);
	
	AI* jennaAI = new AI(map, jenna, character, &aiQueue);
	
	vector<Frame> goldFrames = {
		Frame(glTexImageTGAFile("resources/gold.tga", NULL, NULL), 8, 10), //hacky atm, have a change in character that will be removed later
		Frame(glTexImageTGAFile("resources/gold.tga", NULL, NULL), 8, 10), //assumes 2 is down, as in the future all characters should have all 8 directions
	};
	vector<Animation*> goldAnims = { new Animation(-1, goldFrames) };
	
	//Column* column = new Column(64, 80, 0, 18, 50, glTexImageTGAFile("resources/maps/column.tga", NULL, NULL));
	
	//((TilePushable*)map->getTile(4, 5, 0))->setColumn(column);
	
	vector<Character*> characters = { character, jenna };
	vector<Drawable*> drawables = { (Drawable*)character, (Drawable*)jenna/*, (Drawable*)column*/ };
	vector<AI*> ai = { jennaAI };
}

void Game::load(string levelDir) {
	//	generic load functionality of defaults for level
	
	//	if (level == #) {
	//		load and overwrite special level parameters from separate level save
	//	}
}

void Game::save() {
	//	if (level == #) {
	//		save special level parameters to separate level save
	//	}
}
