#include "core/Timer.h"
#include <cstdio>

using namespace std;
using namespace CR::Core;

Timer::Timer() {
	Reset();
}

/*!
	Reset the total time back to 0. Also resets the last frame time back to 0.
	Generally only used when total time is needed.
	*/
void Timer::Reset() {
	starttime = chrono::high_resolution_clock::now();
}

/*!
	Updates the last frame time, and the total time.
	*/
void Timer::Update() {
	currenttime = chrono::high_resolution_clock::now();
	timeLastFrame = chrono::duration_cast<chrono::microseconds>(currenttime - starttime).count() / 1000000.0;
	starttime = currenttime;
	totalTime += timeLastFrame;
}

void Timer::StartFrame() {
	starttime = chrono::high_resolution_clock::now();
}


ScopedTimer::ScopedTimer(const char* text) : m_text(text) {

}

ScopedTimer::~ScopedTimer() {
	m_timer.Update();
	printf("%s %0.2fms\n", m_text.c_str(), (m_timer.GetTotalTime() * 1000));
}
