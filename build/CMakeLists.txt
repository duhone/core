cmake_minimum_required (VERSION 3.12)
set(CMAKE_CONFIGURATION_TYPES "Debug;ReleaseD;Profile;Final") 

project (core)

###############################################
#options
###############################################

option(IncludeTests "Include Unit Tests" ON)

include (${CMAKE_CURRENT_SOURCE_DIR}/common.cmake)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/build/build_catch.cmake)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/build/build_fmt.cmake)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/build/build_spdlog.cmake)
include (${CMAKE_CURRENT_SOURCE_DIR}/max_warnings.cmake)
include (${CMAKE_CURRENT_SOURCE_DIR}/build.cmake)