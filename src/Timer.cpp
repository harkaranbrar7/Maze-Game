/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */
#include "Timer.h"

Timer::Timer()
{
    //ctor
    startedAt = 0;
	pausedAt = 0;
	paused = false;
	started = false;
}

Timer::~Timer(){
    //dtor
}
 bool Timer::isStarted(){
     return started;
 }
 bool Timer::isStopped(){

     return !started;
 }
 bool Timer::isPaused() {

     return paused;

 }

 void Timer::Pause(){

    if( paused || !started )
		return;
	paused = true;
	pausedAt = clock();
 }

 void Timer::Resume(){

    if( !paused )
		return;
	paused = false;
	startedAt += clock() - pausedAt;
 }


 void Timer::Stop(){

     started = false;
 }

 void Timer::Start(){

     if( started )
		return;
	started = true;
	paused = false;
	startedAt = clock();

 }
 void Timer::Reset(){

    paused = false;
	startedAt = clock();

 }
 clock_t Timer::GetTicks(){

     if(!started)
		return 0;
	if( paused )
		return pausedAt - startedAt;
	return clock() - startedAt;

 }
