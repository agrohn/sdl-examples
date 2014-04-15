////////////////////////////////////////////////////////////////////////////////
// SDL Programming
////////////////////////////////////////////////////////////////////////////////
// Program that demonstrates SDL user event sending & handling.
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
  
  
  SDL_Window *pScreen = SDL_CreateWindow("My Window", 
			      SDL_WINDOWPOS_CENTERED, 
			      SDL_WINDOWPOS_CENTERED, 
			      800,600, 0);

  
  if ( pScreen == NULL )
  {
    throw runtime_error(SDL_GetError());
  }

  SDL_Renderer *renderer = SDL_CreateRenderer( pScreen, -1, SDL_RENDERER_SOFTWARE);
  if ( renderer == NULL ) throw runtime_error( SDL_GetError());

  bool bRunning = true;
  int counter = 42;
  enum { MY_INT_EVENT };

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
	if ( ev.key.keysym.sym == SDLK_RETURN )
	{
	  // allocate new int variable from heap
	  int *value = new int;
	  *value = counter++; 
	  
	  // Create user event
	  SDL_Event uevent;
	  uevent.type = SDL_USEREVENT; //< Type needs to be set to SDL_USEREVENT
	  uevent.user.code = MY_INT_EVENT; //< My own type identifier for the event
	  uevent.user.data1 = value; //< value for the first field
	  uevent.user.data2 = NULL;  //< second field is not used
	  
	  SDL_PushEvent( &uevent ); //< Push event to event queue
	}
	break;
      case SDL_USEREVENT:
	// Here we handle user event
	if ( ev.user.code == MY_INT_EVENT ) // if type is our own type
	{
	  // process data 
	  cout << "Received data: " << *static_cast<int *>(ev.user.data1) << endl;
	  // release memory for dynamically allocated member. This is optional, 
	  // if data is stored also elsewhere.
	  delete static_cast<int *>(ev.user.data1);
	}
	break;
      }
    }
    SDL_RenderPresent(renderer);
  }
  // cleanup
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(pScreen);
  SDL_Quit();
  return 0;
}
