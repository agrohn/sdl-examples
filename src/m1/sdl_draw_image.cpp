// Copyright anssi.grohn@karelia.fi (c) 2014.
// Licenced under GPLv3

#include <SDL.h>    
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
  
  // Attempt to load image
  SDL_Surface *pSurface = SDL_LoadBMP("res/sdl-logo.bmp");
  if ( pSurface == NULL  ) throw runtime_error(SDL_GetError());
  
  
  // Attempt to convert it to texture
  SDL_Texture * pImage = SDL_CreateTextureFromSurface(renderer, pSurface);
  if ( pImage == NULL  ) throw runtime_error(SDL_GetError());
  SDL_FreeSurface( pSurface);
  pSurface = NULL;



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
    SDL_RenderCopy( renderer, pImage, NULL, NULL);
    // flip buffers
    SDL_RenderPresent( renderer );
  }



  SDL_Quit();
  return 0;
}


