/*
 * Timer.h
 *
 *  Created on: 7 lip 2018
 *      Author: Łukasz Pękalski
 */

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

#include "src/main.h"

class Timer {
	public:
		Timer();
		void start();
		void stop();
		void pause();
		void unpause();
		Uint32 getTicks();
		template <typename variable>
		variable getSeconds();
		bool isStarted();
		bool isPaused();
	private:
Uint32 mStartTicks, mPausedTicks;
bool mStarted, mPaused;
};
template <typename variable>
variable Timer::getSeconds() {
	variable time = 0;
	if (mStarted) {
		if (mPaused) {
			time = (mPausedTicks/1000.0);
		} else {
			time = ((SDL_GetTicks() - mStartTicks)/1000.0);
		}
	}
	return time;
}

#endif /* SRC_TIMER_H_ */
