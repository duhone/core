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
    ${root}/inc/core/Random.h
    ${root}/inc/core/ScopeExit.h
    ${root}/inc/core/Span.h
    ${root}/inc/core/storage_buffer.h
    ${root}/inc/core/StringUtil.h
    ${root}/inc/core/Table.h
    ${root}/inc/core/Timer.h
    ${root}/inc/core/TypeTraits.h
    ${root}/inc/core/Reflection.h
    ${root}/inc/core/Log.h
    ${root}/inc/core/Hash.h
)

set(SRCS
    ${root}/src/BinaryStream.cpp
	${root}/src/Function.cpp
    ${root}/src/Guid.cpp
    ${root}/src/Hash.cpp
    ${root}/src/Locked.cpp
    ${root}/src/Log.cpp
    ${root}/src/Misc.cpp
    ${root}/src/Random.cpp
    ${root}/src/Reflection.cpp
    ${root}/src/storage_buffer.cpp
    ${root}/src/StringUtil.cpp
    ${root}/src/Table.cpp
    ${root}/src/TypeTraits.cpp
    ${root}/src/Timer.cpp
)

set(BUILD
    ${root}/build/build.cmake
)

add_library(core 
  ${PUBLIC_HDRS} 
  ${SRCS} 
  ${BUILD}
)

settingsCR(core)	
createPCH(core)

target_include_directories(core PUBLIC "${root}/inc")
target_link_libraries(core PUBLIC
  doctest
  fmt
  function2
  glm
  robinmap
  spdlog
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
	${root}/tests/Table.cpp
	${root}/tests/TypeTraits.cpp
	${root}/tests/main.cpp
)

add_executable(core_tests 
	${SRCS}
)
		
settingsCR(core_tests)
usePCH(core_tests core)
	
target_link_libraries(core_tests 
	doctest
	spdlog
	fmt
	core
)		

set_property(TARGET core_tests APPEND PROPERTY FOLDER tests)
