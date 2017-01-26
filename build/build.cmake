
include_directories("${CMAKE_CURRENT_LIST_DIR}/../inc")

###############################################
#library
###############################################
set(CORE_PUBLIC_HDRS
    ${CMAKE_CURRENT_LIST_DIR}/../inc/core/algorithm.h
    ${CMAKE_CURRENT_LIST_DIR}/../inc/core/ClassFactory.h
    ${CMAKE_CURRENT_LIST_DIR}/../inc/core/Concepts.h
    ${CMAKE_CURRENT_LIST_DIR}/../inc/core/DefaultOperators.h
    ${CMAKE_CURRENT_LIST_DIR}/../inc/core/Function.h
    ${CMAKE_CURRENT_LIST_DIR}/../inc/core/Guid.h
    ${CMAKE_CURRENT_LIST_DIR}/../inc/core/literals.h
    ${CMAKE_CURRENT_LIST_DIR}/../inc/core/Locked.h
    ${CMAKE_CURRENT_LIST_DIR}/../inc/core/ScopeExit.h
    ${CMAKE_CURRENT_LIST_DIR}/../inc/core/Singleton.h
    ${CMAKE_CURRENT_LIST_DIR}/../inc/core/StringUtil.h
    ${CMAKE_CURRENT_LIST_DIR}/../inc/core/Timer.h
    ${CMAKE_CURRENT_LIST_DIR}/../inc/core/TypeTraits.h
)

set(CORE_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/../src/StringUtil.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../src/Timer.cpp
)

set(CORE_BUILD
    ${CMAKE_CURRENT_LIST_DIR}/../build/build.cmake
)

add_library(core OBJECT ${CORE_PUBLIC_HDRS} ${CORE_SRCS} ${CORE_BUILD})
	
source_group("Public Headers" FILES ${CORE_PUBLIC_HDRS})
source_group("Source" FILES ${CORE_SRCS})
source_group("Build" FILES ${CORE_BUILD})
		
if(IncludeTests)  
	###############################################
	#unit tests
	###############################################
set(CORE_TEST_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/../tests/ClassFactory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../tests/ClassFactoryTest1.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../tests/ClassFactoryTest2.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../tests/ClassFactoryTestInterface.h
    ${CMAKE_CURRENT_LIST_DIR}/../tests/Function.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../tests/Guid.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../tests/Locked.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../tests/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../tests/Singleton.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../tests/Misc.cpp
)
	
	add_executable(core_tests $<TARGET_OBJECTS:core> ${CORE_TEST_SRCS})
						
	source_group("Source" FILES ${CORE_TEST_SRCS})

	set_property(TARGET core_tests APPEND PROPERTY FOLDER tests)
endif()
