////////////////////////////////////////////////////////////////////////////////
// SDL Programming
////////////////////////////////////////////////////////////////////////////////
// Example code which limits frame rate.
// Use keys 1, up arrow and down arrow to control drawing.
// Key 1 sets repeat to 1.
// Arrow up doubles repeat.
// Arrow down halves repeat.
// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <SDL2_gfxPrimitives.h>
#include <SDL2_framerate.h>

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
  

  FPSmanager fpsManager;
  SDL_initFramerate( &fpsManager );
  SDL_setFramerate( &fpsManager, 60);
  bool bRunning = true;

  int frameCount = 0;
  Uint32 time = SDL_GetTicks();
  int repeat = 1;
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
      case SDL_KEYDOWN:
	switch ( ev.key.keysym.sym )
	{
	case SDLK_1:
	  repeat = 1;
	  break;
	case SDLK_UP:
	  repeat *= 2;
	  break;
	case SDLK_DOWN:
	  repeat /= 2;
	  break;
	default:
	  break;
	}
	break;
      default:
	break;
      }
    }
    
    for( int i=0;i<repeat;i++)
      boxRGBA(renderer, 0,0,WINDOW_WIDTH,WINDOW_HEIGHT, 255,0,0,255);
    
    // Delay drawing loop
    SDL_framerateDelay(&fpsManager);
    // Determine when one second has passed
    if ( SDL_GetTicks() - time > 1000 )
    {
      cout << "FPS is : " <<  frameCount << " drawing " << repeat << " rectangles." << endl;
      frameCount = 0;
      time = SDL_GetTicks();
    }
    else
      ++frameCount;
    // Put stuff on screen again
    SDL_RenderPresent( renderer );
   
  }
  SDL_Quit();
  return 0;
}
