cmake_minimum_required (VERSION 2.6)

project (core)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

###############################################
#options
###############################################

option(IncludeTests "Include Unit Tests" ON)


include (${CMAKE_CURRENT_SOURCE_DIR}/build.txt)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/build/build.txt)
include_directories("${PROJECT_SOURCE_DIR}/../../3rdParty/catch")