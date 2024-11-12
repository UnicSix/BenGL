cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set( CMAKE_CXX_FLAGS "-g" )
project( BenGL )

set( BenGL_SRC src/*.cpp )

file( GLOB_RECURSE BenGL_SRC "src/*.cpp" )
#file( GLOB_RECURSE IMGUI_SRC "/src/vendor/imgui/i*.cpp" )

add_executable( ben-gl
	#src/vendor/stb_image/stb_image.cpp
	#${IMGUI_SRC}
	${BenGL_SRC}
)

include_directories( "src/" )
include_directories( "src/vendor" )
include_directories( "lib/glm" )
include_directories( "src/vendor/imgui" )

set_property(TARGET ben-gl PROPERTY CXX_STANDARD 17)
target_compile_options( ben-gl PRIVATE -Wall )

add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)

target_link_libraries(ben-gl
  PRIVATE glfw
  PRIVATE libglew_static
  PRIVATE glm
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/src/asset.hpp.in
	${CMAKE_CURRENT_BINARY_DIR}/src/asset.hpp
)

target_include_directories(ben-gl
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
)
