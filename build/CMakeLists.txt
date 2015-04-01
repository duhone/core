cmake_minimum_required (VERSION 2.6)
project (core)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

###############################################
#options
###############################################

option(IncludeTests "Include Unit Tests" ON)

include_directories("${PROJECT_SOURCE_DIR}/../inc/core")

###############################################
#library
###############################################
file(GLOB_RECURSE core_inc "${PROJECT_SOURCE_DIR}/../inc/*.h")
file(GLOB_RECURSE core_src1 "${PROJECT_SOURCE_DIR}/../src/*.h")
file(GLOB_RECURSE core_src2 "${PROJECT_SOURCE_DIR}/../src/*.cpp")
list(APPEND core_src ${core_inc})
list(APPEND core_src ${core_src1})
list(APPEND core_src ${core_src2})

add_library(core OBJECT ${core_src})
					
set_property(TARGET core APPEND PROPERTY COMPILE_FLAGS "-WX -W4")
#set_property(TARGET core APPEND PROPERTY COMPILE_FLAGS "-Werror -Wall -Wextra")	
				
if(IncludeTests)  
	###############################################
	#unit tests
	###############################################
	file(GLOB_RECURSE core_test_inc "${PROJECT_SOURCE_DIR}/../tests/*.h")
	file(GLOB_RECURSE core_test_src1 "${PROJECT_SOURCE_DIR}/../tests/*.cpp")
	list(APPEND core_test_src ${core_test_inc})
	list(APPEND core_test_src ${core_test_src1})
	
	add_executable(core_tests $<TARGET_OBJECTS:core> ${core_test_src})
						
	set_property(TARGET core_tests APPEND PROPERTY COMPILE_FLAGS "-WX -W4")
	#set_property(TARGET tests APPEND PROPERTY COMPILE_FLAGS "-Werror -Wall -Wextra")
endif()
