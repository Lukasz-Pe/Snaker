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
		int getSeconds();
		bool isStarted();
		bool isPaused();
		virtual ~Timer();
	private:
Uint32 mStartTicks, mPausedTicks;
bool mStarted, mPaused;
};

#endif /* SRC_TIMER_H_ */
