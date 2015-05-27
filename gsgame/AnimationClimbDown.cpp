#include "AnimationClimbDown.h"
#include<iostream>

void AnimationClimbDown::update(float deltatime) {
	currentTime = currentTime - deltatime;
	//pre jump
	if (currentTime > .2) {
		if (up) {
			currentFrame = 0;
		}
		else {
			currentFrame = 2;
		}
		//cout << "1\n";
	}
	//mid jump
	else if (currentTime > .1) {
		currentFrame = 1;
		//cout << "2\n";
	}
	//post jump
	else {
		if (up) {
			currentFrame = 2;
		}
		else {
			currentFrame = 0;
		}
		//cout << "3\n";
	}
	//cout << "frame: " << currentFrame <<" current time: " << currentTime << "\n";
}

void AnimationClimbDown::setUp() {
	up = true;
}

void AnimationClimbDown::setDown() {
	up = false;
}

AnimationClimbDown::AnimationClimbDown(float frametime, vector<Frame> frames) : Animation(frametime, frames) {
}


AnimationClimbDown::~AnimationClimbDown() {
}
