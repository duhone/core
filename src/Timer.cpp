#include "Timer.h"

using namespace std;
using namespace CR::Core;

Timer::Timer(void)
{
	Reset();
}

Timer::~Timer(void)
{
}

/*!
	Reset the total time back to 0. Also resets the last frame time back to 0.
	Generally only used when total time is needed.
*/
void Timer::Reset(void)
{
	starttime = chrono::high_resolution_clock::now();
}

/*!
	Updates the last frame time, and the total time. 
*/
void Timer::Update(void)
{
	currenttime = chrono::high_resolution_clock::now();
	timeLastFrame = chrono::duration_cast<chrono::microseconds>(currenttime - starttime).count()/1000000.0;
	starttime = currenttime;
	totalTime += timeLastFrame;
}

void Timer::StartFrame()
{
	starttime = chrono::high_resolution_clock::now();
}