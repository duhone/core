set(core_root "${CMAKE_CURRENT_LIST_DIR}/..")

###############################################
#library
###############################################
set(CORE_PUBLIC_HDRS
    ${core_root}/inc/core/algorithm.h
    ${core_root}/inc/core/ClassFactory.h
    ${core_root}/inc/core/Concepts.h
    ${core_root}/inc/core/DefaultOperators.h
    ${core_root}/inc/core/Function.h
    ${core_root}/inc/core/Guid.h
    ${core_root}/inc/core/literals.h
    ${core_root}/inc/core/Locked.h
    ${core_root}/inc/core/ScopeExit.h
    ${core_root}/inc/core/Span.h
    ${core_root}/inc/core/StringUtil.h
    ${core_root}/inc/core/Timer.h
    ${core_root}/inc/core/TypeTraits.h
    ${core_root}/inc/core/Reflection.h
    ${core_root}/inc/core/Log.h
)

set(CORE_SRCS
    ${core_root}/src/StringUtil.cpp
    ${core_root}/src/Timer.cpp
    ${core_root}/src/Log.cpp
)

set(CORE_BUILD
    ${core_root}/build/common.cmake
    ${core_root}/build/max_warnings.cmake
    ${core_root}/build/build.cmake
)

add_library(core OBJECT 
  ${CORE_PUBLIC_HDRS} 
  ${CORE_SRCS} 
  ${CORE_BUILD}
)
	
source_group("Public Headers" FILES ${CORE_PUBLIC_HDRS})
source_group("Source" FILES ${CORE_SRCS})
source_group("Build" FILES ${CORE_BUILD})
		
target_include_directories(core PUBLIC "${core_root}/inc")
target_link_libraries(core PUBLIC
  spdlog
  fmt
)

if(IncludeTests)  
	###############################################
	#unit tests
	###############################################
  set(CORE_TEST_SRCS
    ${core_root}/tests/ClassFactory.cpp
    ${core_root}/tests/ClassFactoryTest1.cpp
    ${core_root}/tests/ClassFactoryTest2.cpp
    ${core_root}/tests/ClassFactoryTestInterface.h
    ${core_root}/tests/ClassFactoryTestInterface.cpp
    ${core_root}/tests/Function.cpp
    ${core_root}/tests/Guid.cpp
    ${core_root}/tests/Locked.cpp
    ${core_root}/tests/main.cpp
    ${core_root}/tests/Misc.cpp
    ${core_root}/tests/Reflection.cpp
    ${core_root}/tests/Log.cpp
  )
	
  add_executable(core_tests 
    ${CORE_TEST_SRCS}
  )
				
  target_link_libraries(core_tests 
    catch
    spdlog
    fmt
    core
  )		
	source_group("Source" FILES ${CORE_TEST_SRCS})

	set_property(TARGET core_tests APPEND PROPERTY FOLDER tests)
endif()
