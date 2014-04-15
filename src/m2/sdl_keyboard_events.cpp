////////////////////////////////////////////////////////////////////////////////
// SDL Programming
////////////////////////////////////////////////////////////////////////////////
// Program that demonstrates SDL keyboard event handling.
// \author anssi.grohn@karelia.fi (c) 2014
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
  {
    throw runtime_error(SDL_GetError());
  }
  
  
  SDL_Window *pWindow = SDL_CreateWindow( "Input window",
					  SDL_WINDOWPOS_CENTERED,
					  SDL_WINDOWPOS_CENTERED,
					  WINDOW_WIDTH, 
					  WINDOW_HEIGHT, 
					  SDL_WINDOW_SHOWN
					  );
  
  if ( pWindow == NULL )
  {
    throw runtime_error(SDL_GetError());
  }
  bool bRunning = true;
  SDL_Renderer *renderer = SDL_CreateRenderer( pWindow, -1, SDL_RENDERER_SOFTWARE);
  if ( renderer == NULL ) throw runtime_error( SDL_GetError());
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
      case SDL_KEYDOWN: //< When key is pressed down
	switch ( ev.key.keysym.sym )
	{
	case SDLK_q:
	  cout << "Q was pressed, so now we exit!" << endl;
	  bRunning = false;
	  break;
	default:
	  cout << "KEYDOWN event: " ;

	  // in case you are interested filtering out key repeats...
	  if ( ev.key.repeat ) 
	    cout << "(repeating) ";
	  
	  cout << "keysym " << ev.key.keysym.sym << endl;
	  cout << "Key " << SDL_GetKeyName(ev.key.keysym.sym) ;

	  if ( ev.key.keysym.mod & KMOD_SHIFT )
	    cout << ", shift pressed";

	  if ( ev.key.keysym.mod & KMOD_CTRL )
	    cout << ", ctrl pressed";

	  if ( ev.key.keysym.mod & KMOD_ALT )
	    cout << ", alt pressed";
	  cout << endl;
	  break;
	}
	break;
      case SDL_KEYUP: //< When key is released
	cout << "Key up" << endl;
	cout << "KEYUP event: " ;
	cout << "keysym " << ev.key.keysym.sym << endl;
	cout << "Key " << SDL_GetKeyName(ev.key.keysym.sym) ;

	if ( ev.key.keysym.mod & KMOD_SHIFT )
	  cout << ", shift pressed";

	if ( ev.key.keysym.mod & KMOD_CTRL )
	  cout << ", ctrl pressed";

	if ( ev.key.keysym.mod & KMOD_ALT )
	  cout << ", alt pressed";
	cout << endl;
	break;
      }
    }
    SDL_RenderPresent(renderer);
  }
  
  SDL_DestroyRenderer( renderer);
  SDL_DestroyWindow(pWindow);
  SDL_Quit();
  return 0;
}

