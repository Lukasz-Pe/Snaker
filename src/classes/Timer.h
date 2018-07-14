/*
 * Timer.h
 *
 *  Created on: 7 lip 2018
 *      Author: lukasz
 */

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

#include "../main.h"

class Timer {
	public:
		Timer();
		void start();
		void stop();
		void pause();
		void unpause();
		Uint32 getTicks();
//		double getSeconds();
		template <typename zmienna>
		zmienna getSeconds();
		bool isStarted();
		bool isPaused();
		virtual ~Timer();
	private:
Uint32 mStartTicks, mPausedTicks;
bool mStarted, mPaused;
};
template <typename zmienna>
zmienna Timer::getSeconds() {
	zmienna time = 0;
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
