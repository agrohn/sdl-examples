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
find_package(SDL2_gfx REQUIRED)
find_package(TMXParser REQUIRED)
SET( INCLUDEDIRS 
  ${SDL2_INCLUDE_DIR} 
  ${SDL2_TTF_INCLUDE_DIR} 
  ${SDL2_IMAGE_INCLUDE_DIR} 
  ${SDL2_MIXER_INCLUDE_DIR} 
  ${SDL2_GFX_INCLUDE_DIR} 
  ${TMXPARSER_INCLUDE_DIR} 
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
  ${SDL2_GFX_LIBRARY} 
  ${TMXPARSER_LIBRARY} 
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
add_dependencies(draw_image copy)
# --------------------------------------
# Executables to be built for m2
add_executable( draw_image_rect ./src/examples/m2/sdl_draw_image_rect.cpp)
target_link_libraries (draw_image_rect  ${LIBS} )
add_dependencies(draw_image_rect copy)

add_executable( alternative_input ./src/examples/m2/sdl_alternative_input.cpp)
target_link_libraries (alternative_input  ${LIBS} )
add_dependencies(alternative_input copy)

add_executable( draw_image_transparency ./src/examples/m2/sdl_draw_image_transparency.cpp)
target_link_libraries (draw_image_transparency  ${LIBS} )
add_dependencies(draw_image_transparency copy)

add_executable( input ./src/examples/m2/sdl_input.cpp)
target_link_libraries (input  ${LIBS} )
add_dependencies(input copy)

add_executable( keyboard_events ./src/examples/m2/sdl_keyboard_events.cpp)
target_link_libraries (keyboard_events ${LIBS} )
add_dependencies(keyboard_events copy)

add_executable( mouse_events ./src/examples/m2/sdl_mouseevent.cpp)
target_link_libraries (mouse_events ${LIBS} )
add_dependencies(mouse_events copy)

add_executable( user_events ./src/examples/m2/sdl_user_events.cpp)
target_link_libraries (user_events ${LIBS} )
add_dependencies(user_events copy)

add_executable( multiple_windows ./src/examples/m2/sdl_multiple_windows.cpp)
target_link_libraries (multiple_windows ${LIBS} )
add_dependencies(multiple_windows copy)

add_executable( guess ./src/brush-ups/m2/guess.cpp ./src/brush-ups/m2/SDLApp.cpp)
target_link_libraries (guess ${LIBS} )
add_dependencies(guess copy)

# Module 3
add_executable( shapes ./src/examples/m3/sdl_shapes.cpp)
target_link_libraries (shapes ${LIBS} )
add_dependencies(shapes copy)

add_executable( limitframerate ./src/examples/m3/sdl_frameratemgr.cpp)
target_link_libraries (limitframerate ${LIBS} )
add_dependencies(limitframerate copy)

add_executable( fonts ./src/examples/m3/sdl_fonts.cpp)
target_link_libraries (fonts ${LIBS} )
add_dependencies(fonts copy)

add_executable( rendertotexture ./src/examples/m3/sdl_new_texture.cpp)
target_link_libraries (rendertotexture ${LIBS} )
add_dependencies(rendertotexture copy)

add_executable( rotate ./src/examples/m3/sdl_rotate.cpp)
target_link_libraries (rotate ${LIBS} )
add_dependencies(rotate copy)

add_executable( pageexample ./src/examples/m3/pageExample.cpp ./src/examples/m3/Page.cpp ./src/examples/m3/Page.h)
target_link_libraries (pageexample ${LIBS} )
add_dependencies(pageexample copy)

add_executable( limitframerate_gfx ./src/brush-ups/m3/sdl_limitframerate_gfx.cpp)
target_link_libraries (limitframerate_gfx ${LIBS} )
add_dependencies(limitframerate_gfx copy)

add_executable( fadein ./src/brush-ups/m3/fade-in.cpp)
target_link_libraries (fadein ${LIBS} )
add_dependencies(fadein copy)

# Module 4
# --------------------------------------
add_executable( sounds ./src/examples/m4/sdl_sounds.cpp)
target_link_libraries (sounds ${LIBS} )
add_dependencies(sounds copy)

add_executable( sounds2 ./src/examples/m4/sdl_sounds2.cpp)
target_link_libraries (sounds2 ${LIBS} )
add_dependencies(sounds2 copy)

add_executable( sounds3 ./src/examples/m4/sdl_sounds3.cpp)
target_link_libraries (sounds3 ${LIBS} )
add_dependencies(sounds3 copy)

add_executable( sounds_timed ./src/examples/m4/sdl_sounds_timed.cpp)
target_link_libraries (sounds_timed ${LIBS} )
add_dependencies(sounds_timed copy)

add_executable( sounds_timed2 ./src/examples/m4/sdl_sounds_timed2.cpp)
target_link_libraries (sounds_timed2 ${LIBS} )
add_dependencies(sounds_timed2 copy)

add_executable( sounds_fade ./src/examples/m4/sdl_sounds_fade.cpp)
target_link_libraries (sounds_fade ${LIBS} )
add_dependencies(sounds_fade copy)

add_executable( sounds_halthook ./src/examples/m4/sdl_sounds_halthook.cpp)
target_link_libraries (sounds_halthook ${LIBS} )
add_dependencies(sounds_halthook copy)

add_executable( music ./src/examples/m4/sdl_music.cpp)
target_link_libraries (music ${LIBS} )
add_dependencies(music copy)

add_executable( music_controls ./src/examples/m4/sdl_music2.cpp)
target_link_libraries (music_controls ${LIBS} )
add_dependencies(music_controls copy)

add_executable( music_fade ./src/examples/m4/sdl_music3.cpp)
target_link_libraries (music_fade ${LIBS} )
add_dependencies(music_fade copy)

add_executable( sfx_panning ./src/examples/m4/sdl_sfx_panning.cpp)
target_link_libraries (sfx_panning ${LIBS} )
add_dependencies(sfx_panning copy)

add_executable( sfx_revstereo ./src/examples/m4/sdl_sfx_revstereo.cpp)
target_link_libraries (sfx_revstereo ${LIBS} )
add_dependencies(sfx_revstereo copy)

add_executable( sfx_position ./src/examples/m4/sdl_sfx_position.cpp)
target_link_libraries (sfx_position ${LIBS} )
add_dependencies(sfx_position copy)
# module 5
add_executable( animate ./src/examples/m5/sdl_animate.cpp)
target_link_libraries (animate ${LIBS} )

add_executable( animate_framerate ./src/examples/m5/sdl_animate_framerate.cpp)
target_link_libraries (animate_framerate ${LIBS} )
add_dependencies(animate copy)

add_executable( background ./src/examples/m5/sdl_background.cpp)
target_link_libraries (background ${LIBS} )
add_dependencies(background copy)

add_executable( background2 ./src/examples/m5/sdl_background2.cpp)
target_link_libraries (background2 ${LIBS} )
add_dependencies(background2 copy)
# module 6
add_executable( collision_circles ./src/examples/m6/sdl_collision_circles.cpp)
target_link_libraries (collision_circles ${LIBS} )
add_dependencies(collision_circles copy)

add_executable( collision_line_circle ./src/examples/m6/sdl_collision_line_circle.cpp)
target_link_libraries (collision_line_circle ${LIBS} )
add_dependencies(collision_line_circle copy)

add_executable( collision_line_rect ./src/examples/m6/sdl_collision_line_rect.cpp)
target_link_libraries (collision_line_rect ${LIBS} )
add_dependencies(collision_line_rect copy)

add_executable( collision_rectangles ./src/examples/m6/sdl_collision_rectangles.cpp)
target_link_libraries (collision_rectangles ${LIBS} )
add_dependencies(collision_rectangles copy)

add_executable( collision_per_pixel ./src/examples/m6/sdl_collision_per_pixel.cpp)
target_link_libraries (collision_per_pixel ${LIBS} )
add_dependencies(collision_per_pixel copy)

add_executable( tiles ./src/examples/m6/sdl_tiles.cpp)
target_link_libraries (tiles ${LIBS} )
add_dependencies(tiles copy)

add_executable( tmxtiles ./src/examples/m6/sdl_tiles_tmxparser.cpp)
target_link_libraries (tmxtiles ${LIBS} )
add_dependencies(tmxtiles copy)
# module 7
add_executable( speed ./src/examples/m7/sdl_speed.cpp)
target_link_libraries (speed ${LIBS} )
add_dependencies(speed copy)

add_executable( acceleration ./src/examples/m7/sdl_acceleration.cpp)
target_link_libraries (acceleration ${LIBS} )
add_dependencies(acceleration copy)

add_executable( gravity ./src/examples/m7/sdl_gravity.cpp)
target_link_libraries (gravity ${LIBS} )
add_dependencies(gravity copy)

add_executable( deadlock ./src/examples/m7/sdl_deadlock.cpp)
target_link_libraries (deadlock ${LIBS} )
add_dependencies(deadlock copy)

add_executable( mutex ./src/examples/m7/sdl_mutex.cpp)
target_link_libraries (mutex ${LIBS} )
add_dependencies(mutex copy)

add_executable( mutex2 ./src/examples/m7/sdl_mutex2.cpp)
target_link_libraries (mutex2 ${LIBS} )
add_dependencies(mutex2 copy)



add_executable( threads ./src/examples/m7/sdl_threads.cpp)
target_link_libraries (threads ${LIBS} )
add_dependencies(threads copy)

add_executable( threads2 ./src/examples/m7/sdl_threads2.cpp)
target_link_libraries (threads2 ${LIBS} )
add_dependencies(threads2 copy)

add_executable( particles ./src/examples/m7/sdl_particles.cpp ./src/examples/m7/CTimer.cpp)
target_link_libraries (particles ${LIBS} )
add_dependencies(particles copy)

add_executable( particles2 ./src/examples/m7/sdl_particles2.cpp ./src/examples/m7/CTimer.cpp)
target_link_libraries (particles2 ${LIBS} )
add_dependencies(particles2 copy)
