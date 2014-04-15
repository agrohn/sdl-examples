////////////////////////////////////////////////////////////////////////////////
/// SDL Programming
////////////////////////////////////////////////////////////////////////////////
/// Program that opens SDL graphics window, and receives couple of different types 
/// of events, and reports them to user.
/// \author anssi.grohn@karelia.fi (c) 2014.
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <iostream> 
#include <stdexcept> 

using namespace std;
const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;

int main( int argc, char **argv )
{

  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    throw runtime_error( SDL_GetError());

  
  
  SDL_Window *pScreen = SDL_CreateWindow( "SDL Input handling example",
					  SDL_WINDOWPOS_CENTERED,
					   SDL_WINDOWPOS_CENTERED,
					   WINDOW_WIDTH, 
					   WINDOW_HEIGHT, 
					  SDL_WINDOW_SHOWN
					  );
  

  if ( pScreen == NULL ) throw runtime_error( SDL_GetError());


  SDL_Renderer *renderer = SDL_CreateRenderer( pScreen, -1, SDL_RENDERER_SOFTWARE);
  if ( renderer == NULL ) throw runtime_error( SDL_GetError());


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
      case SDL_KEYUP:
	cout << "Key down" << endl;
	break;
      case SDL_KEYDOWN:
	cout << "Key up" << endl;
	break;
      case SDL_MOUSEBUTTONDOWN:
	cout << "Mousebutton down" << endl;
	break;
      case SDL_MOUSEBUTTONUP:
	cout << "Mousebutton up" << endl;
	break;
      case SDL_MOUSEMOTION:
	cout << "Mouse moved! Eek!" << endl;
	break;
      case SDL_JOYAXISMOTION:
	cout << "Joystick input! Yeah!" << endl;
      break;
      }
    }
    
    
    SDL_RenderPresent( renderer );
  }
  SDL_DestroyRenderer( renderer);
  SDL_DestroyWindow(pScreen);
  SDL_Quit();
  
  return 0;
}
