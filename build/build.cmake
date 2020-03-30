set(root "${CMAKE_CURRENT_LIST_DIR}/..")

###############################################
#library
###############################################
set(PUBLIC_HDRS
    ${root}/inc/core/algorithm.h
    ${root}/inc/core/BinaryStream.h
    ${root}/inc/core/ClassFactory.h
    ${root}/inc/core/Concepts.h
    ${root}/inc/core/DefaultOperators.h
    ${root}/inc/core/FileHandle.h
    ${root}/inc/core/Function.h
    ${root}/inc/core/Guid.h
    ${root}/inc/core/literals.h
    ${root}/inc/core/Locked.h
    ${root}/inc/core/ScopeExit.h
    ${root}/inc/core/Span.h
    ${root}/inc/core/StringUtil.h
    ${root}/inc/core/Timer.h
    ${root}/inc/core/TypeTraits.h
    ${root}/inc/core/Reflection.h
    ${root}/inc/core/Log.h
    ${root}/inc/core/Hash.h
)

set(SRCS
    ${root}/src/StringUtil.cpp
    ${root}/src/Timer.cpp
    ${root}/src/Log.cpp
    ${root}/src/Hash.cpp
)

set(BUILD
    ${root}/build/build.cmake
)

add_library(core OBJECT 
  ${PUBLIC_HDRS} 
  ${SRCS} 
  ${BUILD}
)

settingsCR(core)	
		
target_include_directories(core PUBLIC "${root}/inc")
target_link_libraries(core PUBLIC
  spdlog
  fmt
)

###############################################
#unit tests
###############################################
set(SRCS
	${root}/tests/ClassFactory.cpp
	${root}/tests/ClassFactoryTest1.cpp
	${root}/tests/ClassFactoryTest2.cpp
	${root}/tests/ClassFactoryTestInterface.h
	${root}/tests/ClassFactoryTestInterface.cpp
	${root}/tests/Function.cpp
	${root}/tests/Guid.cpp
	${root}/tests/Locked.cpp
	${root}/tests/main.cpp
	${root}/tests/Misc.cpp
	${root}/tests/Reflection.cpp
	${root}/tests/Log.cpp
)

add_executable(core_tests 
	${SRCS}
)
		
settingsCR(core_tests)
	
target_link_libraries(core_tests 
	catch
	spdlog
	fmt
	core
)		

set_property(TARGET core_tests APPEND PROPERTY FOLDER tests)
