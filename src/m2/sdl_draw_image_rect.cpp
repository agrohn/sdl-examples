// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licenced under GPLv3
#include <SDL.h>    
#include <SDL_image.h>
#include <stdexcept>
#include <iostream> 
using namespace std;
const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;
const int COLOR_DEPTH_IN_BITS = 24;

int main( int argc, char **argv )
{
  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
  {
    std::cerr << "Init failed:" 
              << SDL_GetError() << std::endl;
    return 1;
  }
  SDL_Window *pScreen = SDL_CreateWindow("Hello", 
					 0,0,
					 WINDOW_WIDTH, 
					 WINDOW_HEIGHT, 
					 SDL_WINDOW_SHOWN
					 );
  
  SDL_Renderer *renderer = SDL_CreateRenderer( pScreen, -1,  SDL_RENDERER_ACCELERATED);
  if ( pScreen == NULL )
  {
    std::cerr << "Could not open window: " 
              << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }
  
  
  // Attempt to convert it to texture
  SDL_Texture * pImage = IMG_LoadTexture(renderer, "res/sdl-logo.png");
  if ( pImage == NULL  ) throw runtime_error(SDL_GetError());
  
  int w,h;
  SDL_QueryTexture(pImage,NULL, NULL, &w, &h );
  // Select part of bitmap to be drawn
  // (actually, it is the entire bitmap)
  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  src.w = w;
  src.h = h;

  // Select are on screen where image is drawn.
  // (w and h in this case do not matter, since we do not scale image.)
  SDL_Rect dest;

  dest.x = 120;
  dest.y = 220;
  dest.w = w;
  dest.h = h;
  bool bRunning = true;
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
  SDL_SetTextureBlendMode( pImage, SDL_BLENDMODE_BLEND);
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
    
    SDL_RenderClear(renderer);
    // Draw image on screen
    SDL_RenderCopy( renderer, pImage, &src, &dest);
    // flip buffers
    SDL_RenderPresent( renderer );
  }
  // Release image 
  //SDL_FreeSurface(pImage );


  SDL_Quit();
  return 0;
}

