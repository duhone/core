cmake_minimum_required (VERSION 3.16)
set(CMAKE_CONFIGURATION_TYPES "Debug;RelWithDebInfo;Profile;Final") 

project (core)

include (${CMAKE_CURRENT_SOURCE_DIR}/../../build/common.cmake)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/build/build_doctest.cmake)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/build/build_fmt.cmake)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/build/build_function2.cmake)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/build/build_spdlog.cmake)
include (${CMAKE_CURRENT_SOURCE_DIR}/build.cmake)