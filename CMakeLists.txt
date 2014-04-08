cmake_minimum_required(VERSION 2.8)
project(quickescape )
# Use also project's custom cmake modules.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
SET(CMAKE_BUILD_TYPE Debug)

if(UNIX)
  add_definitions("--std=c++11 -Wall")
endif()


find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

SET( INCLUDEDIRS 
  ${SDL2_INCLUDE_DIR} 
  ${SDL2_TTF_INCLUDE_DIR} 
  ${SDL2_IMAGE_INCLUDE_DIR} 
  ${SDL2_MIXER_INCLUDE_DIR} 
  ${TINYXML2_INCLUDE_DIR} 
 ./src/m1
 ./src/m2
 ./src/m3
 ./src/m4
 ./src/m5
 ./src/m6
 ./src/m7
 )

SET( LIBS 
  ${SDL2_LIBRARY} 
  ${SDL2_TTF_LIBRARY} 
  ${SDL2_IMAGE_LIBRARY} 
  ${TINYXML2_LIBRARY} 
  ${SDL2_MIXER_LIBRARY} 
)

include_directories( quickescape ${INCLUDEDIRS} )

add_custom_target(copy)
add_custom_command( TARGET copy PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res)
# --------------------------------------
# Executable to be built
add_executable( draw_image ./src/m1/sdl_draw_image.cpp)
target_link_libraries (draw_image ${LIBS} )

add_executable( draw_image_rect ./src/m2/sdl_draw_image_rect.cpp)
target_link_libraries (draw_image_rect  ${LIBS} )

# --------------------------------------
# dependencies
add_dependencies(draw_image copy)

