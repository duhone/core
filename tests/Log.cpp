#include "core/Log.h"
#include "catch.hpp"
#include <thread>

using namespace CR::Core;
using namespace std;
using namespace std::literals;

TEST_CASE("Log", "") {
	Log::Info("Testing info");
	Log::Warn("Testing warn");

	// Leave commented out, will crash(on purpose) in debug and release builds
	// Log::Error("Testing Error");

	// Leave commented out, will crash(on purpose) in all builds
	// Log::Fail("Testing Fail");

	// catch logging interferes with spdlog, so give a little bit of time for spdlog to finish
	this_thread::sleep_for(250ms);
}
