cmake_minimum_required (VERSION 2.6)
project (core)

find_package (GTest)

###############################################
#options
###############################################

if(GTEST_FOUND) 
	option(IncludeTests "Include Unit Tests" ON)
endif()

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
						
				
if(IncludeTests AND GTEST_FOUND)  
	###############################################
	#gtest dependency
	#needs to be built as static library, with shared crt
	###############################################		
	#add_library( gtest STATIC IMPORTED )
	#set_target_properties( gtest PROPERTIES IMPORTED_LOCATION_DEBUG ${GtestDebug}/gtest.lib )
	#set_target_properties( gtest PROPERTIES IMPORTED_LOCATION_RELEASE ${GtestRelease}/gtest.lib )
	#set_target_properties( gtest PROPERTIES IMPORTED_LOCATION_RELWITHDEBINFO ${GtestRelease}/gtest.lib )

	###############################################
	#unit tests
	###############################################
	add_executable(tests "${PROJECT_SOURCE_DIR}/../tests/Singleton.cpp"
						"${PROJECT_SOURCE_DIR}/../tests/guid.cpp"
						"${PROJECT_SOURCE_DIR}/../tests/ClassFactoryTestInterface.h"
						"${PROJECT_SOURCE_DIR}/../tests/ClassFactoryTest1.cpp"
						"${PROJECT_SOURCE_DIR}/../tests/ClassFactoryTest2.cpp"
						"${PROJECT_SOURCE_DIR}/../tests/ClassFactory.cpp"
						"${PROJECT_SOURCE_DIR}/../tests/Function.cpp"
						"${PROJECT_SOURCE_DIR}/../tests/Locked.cpp"
						"${PROJECT_SOURCE_DIR}/../tests/main.cpp")
	target_include_directories(tests PRIVATE "${GtestInclude}")
	include_directories(${GTEST_INCLUDE_DIRS})
	target_link_libraries(tests ${GTEST_LIBRARIES})
endif()