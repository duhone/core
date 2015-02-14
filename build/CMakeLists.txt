cmake_minimum_required (VERSION 2.6)
project (core)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

###############################################
#options
###############################################

option(IncludeTests "Include Unit Tests" ON)

include_directories("${PROJECT_SOURCE_DIR}/../inc")

###############################################
#library
###############################################
add_library(core OBJECT "${PROJECT_SOURCE_DIR}/../inc/algorithm.h"
						"${PROJECT_SOURCE_DIR}/../inc/ClassFactory.h"
						"${PROJECT_SOURCE_DIR}/../inc/FSM.h"
						"${PROJECT_SOURCE_DIR}/../inc/Function.h"
						"${PROJECT_SOURCE_DIR}/../inc/Guid.h"
						"${PROJECT_SOURCE_DIR}/../inc/Property.h"
						"${PROJECT_SOURCE_DIR}/../inc/Singleton.h"
						"${PROJECT_SOURCE_DIR}/../inc/StringUtil.h"
						"${PROJECT_SOURCE_DIR}/../inc/Timer.h"
						"${PROJECT_SOURCE_DIR}/../inc/OnScopeExit.h"
						"${PROJECT_SOURCE_DIR}/../inc/Locked.h"
						"${PROJECT_SOURCE_DIR}/../inc/Concepts.h"
						"${PROJECT_SOURCE_DIR}/../inc/DefaultOperators.h"
						"${PROJECT_SOURCE_DIR}/../src/StringUtil.cpp"
						"${PROJECT_SOURCE_DIR}/../src/Timer.cpp")
						
				
if(IncludeTests)  
	###############################################
	#unit tests
	###############################################
	add_executable(tests "${PROJECT_SOURCE_DIR}/../tests/Singleton.cpp"
						"${PROJECT_SOURCE_DIR}/../tests/Guid.cpp"
						"${PROJECT_SOURCE_DIR}/../tests/ClassFactoryTestInterface.h"
						"${PROJECT_SOURCE_DIR}/../tests/ClassFactoryTest1.cpp"
						"${PROJECT_SOURCE_DIR}/../tests/ClassFactoryTest2.cpp"
						"${PROJECT_SOURCE_DIR}/../tests/ClassFactory.cpp"
						"${PROJECT_SOURCE_DIR}/../tests/Function.cpp"
						"${PROJECT_SOURCE_DIR}/../tests/Locked.cpp"
						"${PROJECT_SOURCE_DIR}/../tests/main.cpp")
endif()
