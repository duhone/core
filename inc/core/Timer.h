﻿#pragma once
#include <chrono>
#include <string>

namespace CR::Core {
	//! A high resolution timer.
	/*!
	    Uses high resolution performance counter to implement a timer. High precesion.
	    All returned values are in seconds. You should only call Update once per frame.
	    The LastFrameTime is always the amount of time between the last 2 calls to update.

	    Generally use GetTotalTime and Reset to keep track of the amount of time of a single event.
	    Use Update only when creating a frame rate type counter.
	    */
	class Timer {
	  public:
		Timer();
		//! Total time since timer was created or Reset was called
		/*!
		    This will be set back to 0 every time Reset is called.
		    */
		double GetTotalTime() const { return totalTime; }
		//! Time between last 2 calls to Update
		/*!
		    Don't use Reset if using this call and Update. Reset will set this back
		    to 0, and it won't change from that until after the next Update call.
		    */
		double GetLastFrameTime() const { return timeLastFrame; }
		//! Reset the total time back to 0.
		void Reset();
		void StartFrame();
		//! Next Frame. Update the LastFrameTime
		void Update();

	  private:
		std::chrono::high_resolution_clock::time_point starttime;
		std::chrono::high_resolution_clock::time_point currenttime;

		double timerFreqInv{0.0};
		double totalTime{0.0};
		double timeLastFrame{0.0};
	};

	class ScopedTimer {
	  public:
		ScopedTimer(const char* text);
		~ScopedTimer();

	  private:
		Timer m_timer;
		std::string m_text;
	};
}    // namespace CR::Core
