cmake_minimum_required(VERSION 2.8)
project(sdl-examples )
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

 ./src/examples/m1
 ./src/examples/m2
 ./src/examples/m3
 ./src/examples/m4
 ./src/examples/m5
 ./src/examples/m6
 ./src/examples/m7
 ./src/brush-ups/m1
 ./src/brush-ups/m2
 ./src/brush-ups/m3
 ./src/brush-ups/m4
 ./src/brush-ups/m5
 ./src/brush-ups/m6
 ./src/brush-ups/m7
 )

SET( LIBS 
  ${SDL2_LIBRARY} 
  ${SDL2_TTF_LIBRARY} 
  ${SDL2_IMAGE_LIBRARY} 
  ${SDL2_MIXER_LIBRARY} 
)

file(GLOB_RECURSE DLLS *.dll)
include_directories( sdl-examples ${INCLUDEDIRS} )

add_custom_target(copy)
add_custom_command( TARGET copy PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/res ${CMAKE_CURRENT_BINARY_DIR}/res)


# Windows needs its dlls.
# do the copying
foreach( file_i ${DLLS})
    add_custom_command(
    TARGET copy
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy ${file_i} ${CMAKE_CURRENT_BINARY_DIR}
)
endforeach( file_i )



# --------------------------------------
# Executable to be built for m1
add_executable( draw_image ./src/examples/m1/sdl_draw_image.cpp)
target_link_libraries (draw_image ${LIBS} )

# --------------------------------------
# Executables to be built for m2
add_executable( draw_image_rect ./src/examples/m2/sdl_draw_image_rect.cpp)
target_link_libraries (draw_image_rect  ${LIBS} )

add_executable( alternative_input ./src/examples/m2/sdl_alternative_input.cpp)
target_link_libraries (alternative_input  ${LIBS} )

add_executable( draw_image_transparency ./src/examples/m2/sdl_draw_image_transparency.cpp)
target_link_libraries (draw_image_transparency  ${LIBS} )

add_executable( input ./src/examples/m2/sdl_input.cpp)
target_link_libraries (input  ${LIBS} )

add_executable( keyboard_events ./src/examples/m2/sdl_keyboard_events.cpp)
target_link_libraries (keyboard_events ${LIBS} )

add_executable( mouse_events ./src/examples/m2/sdl_mouseevent.cpp)
target_link_libraries (mouse_events ${LIBS} )

add_executable( user_events ./src/examples/m2/sdl_user_events.cpp)
target_link_libraries (user_events ${LIBS} )

add_executable( multiple_windows ./src/examples/m2/sdl_multiple_windows.cpp)
target_link_libraries (multiple_windows ${LIBS} )

add_executable( guess ./src/brush-ups/m2/guess.cpp ./src/brush-ups/m2/SDLApp.cpp)
target_link_libraries (guess ${LIBS} )
# --------------------------------------
# dependencies
add_dependencies(draw_image copy)

