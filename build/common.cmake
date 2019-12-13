set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)

add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/arch:AVX>)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/fp:fast>)
add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/fp:except->)
add_compile_options($<$<CXX_COMPILER_ID:Clang>:-ffast-math>)
add_compile_options($<$<CXX_COMPILER_ID:GNU>:-ffast-math>)

add_compile_options($<$<AND:$<CXX_COMPILER_ID:MSVC>,$<OR:$<CONFIG:ReleaseD>,$<CONFIG:Profile>,$<CONFIG:Final>>>:/Oi>)
add_compile_options($<$<AND:$<CXX_COMPILER_ID:MSVC>,$<OR:$<CONFIG:ReleaseD>,$<CONFIG:Profile>,$<CONFIG:Final>>>:/Ot>)
add_compile_options($<$<AND:$<CXX_COMPILER_ID:MSVC>,$<OR:$<CONFIG:ReleaseD>,$<CONFIG:Profile>,$<CONFIG:Final>>>:/sdl->)

add_compile_definitions($<$<CONFIG:Debug>:CR_DEBUG>)
add_compile_definitions($<$<CONFIG:ReleaseD>:CR_RELEASE>)
add_compile_definitions($<$<CONFIG:Profile>:CR_PROFILE>)
add_compile_definitions($<$<CONFIG:Final>:CR_FINAL>)
add_compile_definitions($<$<AND:$<CXX_COMPILER_ID:MSVC>,$<OR:$<CONFIG:Profile>,$<CONFIG:Final>>>:NDEBUG>)