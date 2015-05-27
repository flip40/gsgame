#include "AnimationClimbUp.h"
#include<iostream>

void AnimationClimbUp::update(float deltatime) {
	currentTime = currentTime - deltatime;
	//pre jump
	if (currentTime > .3) {
		if (up) {
			currentFrame = 3;
		}
		else {
			currentFrame = 0;
		}
		//cout << "1\n";
	}
	//mid jump
	else if (currentTime > .2) {
		if (up) {
			currentFrame = 2;
		}
		else {
			currentFrame = 1;
		}
		//cout << "2\n";
	}
	//mid jump
	else if (currentTime > .1) {
		if (up) {
			currentFrame = 1;
		}
		else {
			currentFrame = 2;
		}
		//cout << "2\n";
	}
	//post jump
	else {
		if (up) {
			currentFrame = 0;
		}
		else {
			currentFrame = 3;
		}
		//cout << "3\n";
	}
	//cout << "frame: " << currentFrame << " up: " << up << " current time: " << currentTime << "\n";
}

void AnimationClimbUp::setUp() {
	up = true;
}

void AnimationClimbUp::setDown() {
	up = false;
}

AnimationClimbUp::AnimationClimbUp(float frametime, vector<Frame> frames) : Animation(frametime, frames) {
}


AnimationClimbUp::~AnimationClimbUp() {
}
