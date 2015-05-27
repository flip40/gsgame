#include "AnimationJump.h"
#include<iostream>

void AnimationJump::update(float deltatime) {
	currentTime = currentTime - deltatime;
	//pre jump
	if (currentTime > .4) {
		currentFrame = 0;
		//cout << "1\n";
	}
	//mid jump
	else if (currentTime > .1) {
		currentFrame = 1;
		//cout << "2\n";
	}
	//post jump
	else {
		currentFrame = 2;
		//cout << "3\n";
	}
	//cout << "current time: " << currentTime << "\n";
}

AnimationJump::AnimationJump(float frametime, vector<Frame> frames) : Animation(frametime, frames) {
}


AnimationJump::~AnimationJump() {
}
