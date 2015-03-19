////////////////////////////////////////////////////////////////////////////////
// SDL Programming 
////////////////////////////////////////////////////////////////////////////////
// Example of rotation and scaling using SDL_RenderCopyEx.
// \author anssi.grohn@karelia.fi (c) 2014-2015.
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <SDL_image.h>
#include <SDL2_rotozoom.h>
#include <iostream> 
#include <stdexcept>

const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;

using namespace std;

int main( int argc, char **argv )
{
  
  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
  {
    throw runtime_error(SDL_GetError());
  }
  
  SDL_Window *window = SDL_CreateWindow( "SDL_ttf example",
					 SDL_WINDOWPOS_CENTERED,
					 SDL_WINDOWPOS_CENTERED,
					 WINDOW_WIDTH, 
					 WINDOW_HEIGHT, 
					 SDL_WINDOW_SHOWN
					 );
  
  if ( window == NULL )
    throw runtime_error(SDL_GetError());

  SDL_Renderer *renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_SOFTWARE);

  if ( renderer == NULL )
    throw runtime_error(SDL_GetError());

  // Load images
  SDL_Texture * pImage  = IMG_LoadTexture(renderer,"./res/plane.png");
  float fRotation = 0.0f;
  float fScale = 0.1f;

  if ( pImage == NULL  )
    throw runtime_error(IMG_GetError());
  


  bool bRunning = true;
  while ( bRunning )
  {

    SDL_Event ev;

    while ( SDL_PollEvent( & ev ))  // Check event queue.
    {
      switch( ev.type )
      {
      case SDL_QUIT: // when window is closed
	bRunning = false;
	break;
      }
    }
      
    SDL_Rect dest;
    dest.x = 220;
    dest.y = 220;  

    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);
    // Determine width and height for texture in pixels.
    SDL_QueryTexture( pImage, NULL, NULL, &dest.w, &dest.h);
    // scale texture
    dest.w *= fScale;
    dest.h *= fScale;
    // center it
    dest.x -= dest.w/2;
    dest.y -= dest.h/2;

    // Compute rotation pivot point 
    // (actually, if you pass NULL to RenderCopyEx, logical center will be the default action anyway)
    SDL_Point center = { dest.w/2, dest.h/2};    

    // Render image to screen
    SDL_RenderCopyEx( renderer, pImage, NULL, &dest, fRotation, &center, SDL_FLIP_NONE);
    SDL_RenderPresent( renderer );
 
    // Alter rotation and scaling values.
    fRotation += 0.5f;
    if ( fRotation > 360.0f ) fRotation -= 360.0f;
    fScale+=0.001f;
    if ( fScale > 3.0f ) fScale = 0.1f;
  }

  SDL_DestroyTexture( pImage );
  SDL_Quit();
  
  return 0;
}
