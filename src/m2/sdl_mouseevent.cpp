////////////////////////////////////////////////////////////////////////////////
// SDL Programming 
////////////////////////////////////////////////////////////////////////////////
// Program that demonstrates SDL mouse event handling.
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
    std::cerr << "Init failed:" 
              << SDL_GetError() << std::endl;
    return 1;
  }
  
  SDL_Window *pScreen = SDL_CreateWindow("My Window", 
					 SDL_WINDOWPOS_CENTERED, 
					 SDL_WINDOWPOS_CENTERED, 
					 WINDOW_WIDTH,WINDOW_HEIGHT, 
					 SDL_WINDOW_SHOWN);

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
      case SDL_MOUSEBUTTONDOWN: //< Mouse button is pressed down
	cout << "Mouse button ";
	switch( ev.button.button )
	{
	case SDL_BUTTON_LEFT:
	  cout << "LEFT"; 
	  break;
	case SDL_BUTTON_RIGHT:
	  cout << "RIGHT"; 
	  break;
	case SDL_BUTTON_MIDDLE:
	  cout << "MIDDLE"; 
	  break;
	case SDL_BUTTON_X1:
	  cout << "X1"; 
	  break;
	case SDL_BUTTON_X2:
	  cout << "X2"; 
	  break;
	}
	cout << "  PRESSED at (" << ev.button.x << "," << ev.button.y << ")" 
	     << endl;
	break;
      case SDL_MOUSEBUTTONUP: //< Mouse button is released.

	cout << "Mouse button ";
	switch( ev.button.button )
	{
	case SDL_BUTTON_LEFT:
	  cout << "LEFT"; 
	  break;
	case SDL_BUTTON_RIGHT:
	  cout << "RIGHT"; 
	  break;
	case SDL_BUTTON_MIDDLE:
	  cout << "MIDDLE"; 
	  break;
	case SDL_BUTTON_X1:
	  cout << "X1"; 
	  break;
	case SDL_BUTTON_X2:
	  cout << "X2"; 
	  break;
	}
	cout << "  RELEASED at (" 
	     << ev.button.x << "," << ev.button.y << ")"  << endl;
	break;
      case SDL_MOUSEWHEEL: //< mouse wheel has its own event identifier, access it like this.
	cout << "mouse scroll horizontal: " << ev.wheel.x << "\n";
	cout << "mouse scroll vertical: " << ev.wheel.y << "\n";
	break;
      case SDL_MOUSEMOTION: //< When mouse moves, you get mouse motion.
	cout << "current mouse position (" 
	     << ev.motion.x << "," << ev.motion.y << ")";
	cout << ", relative move  (" 
	     << ev.motion.xrel << "," << ev.motion.yrel << ").";
	cout << "Buttons held : ";

	// Examine button states while moving like this
	if ( ev.motion.state & SDL_BUTTON( SDL_BUTTON_LEFT) )
	{
	  cout << "LEFT ";
	}
	if ( ev.motion.state & SDL_BUTTON( SDL_BUTTON_MIDDLE) )
	{
	  cout << "MIDDLE ";
	}
	if ( ev.motion.state & SDL_BUTTON( SDL_BUTTON_RIGHT ) )
	{
	  cout << "RIGHT ";
	}
	cout << endl;
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
