//=============================================================================
// Author:   Arvind
// Purpose:  Timer class
//=============================================================================
#ifndef TIMER_H
#define TIMER_H

#ifdef __GNUC__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

class Timer
{
private:
	//The clock time when the timer started
	Uint32 startTicks;

	//The ticks stored when the timer was paused
	Uint32 pausedTicks;

	//The timer status
	bool paused;
	bool started;

public:
	//Initializes variables
	Timer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	Uint32 get_ticks();

	//Checks the status of the timer
	bool Started(){ return started; }
	bool Paused(){ return paused; }
};

#endif //TIMER_H