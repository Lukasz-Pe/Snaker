/*
 * Timer.cpp
 *
 *  Created on: 7 lip 2018
 *      Author: lukasz
 */

#include "Timer.h"

Timer::Timer() {
	mStartTicks = 0;
	mPausedTicks = 0;
	mPaused = false;
	mStarted = false;
}

void Timer::start() {
	mStarted = true;
	mPaused = false;
	mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void Timer::stop() {
	mStarted = false;
	mPaused = false;
	mStartTicks = 0;
	mPausedTicks = 0;
}

void Timer::pause() {
	if (mStarted && !mPaused) {
		mPaused = true;
		mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
	}
}

void Timer::unpause() {
	if (mStarted && mPaused) {
		mPaused = false;
		mStartTicks = SDL_GetTicks() - mPausedTicks;
		mPausedTicks = 0;
	}
}

Uint32 Timer::getTicks() {
	Uint32 time = 0;
	if (mStarted) {
		if (mPaused) {
			time = mPausedTicks;
		} else {
			time = SDL_GetTicks() - mStartTicks;
		}
	}

	return time;
}
/*double Timer::getSeconds() {
	double time = 0;
	if (mStarted) {
		if (mPaused) {
			time = (mPausedTicks/1000.0);
		} else {
			time = ((SDL_GetTicks() - mStartTicks)/1000.0);
		}
	}
	return time;
}*/

bool Timer::isStarted() {
	return mStarted;
}

bool Timer::isPaused() {
	return mPaused && mStarted;
}

Timer::~Timer() {

}

