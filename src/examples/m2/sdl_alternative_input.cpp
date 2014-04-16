////////////////////////////////////////////////////////////////////////////////
// SDL Programming 
////////////////////////////////////////////////////////////////////////////////
// Program that demonstrates alternative input handling.
// \author anssi.grohn@karelia.fi (c) 2014.
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
  
  
  // Open window
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

  while ( bRunning )
  {
    // update device states
    SDL_PumpEvents();
    // access keyboard state
    const Uint8 *pKeyState = SDL_GetKeyboardState(NULL);

    // access keys via scancode 
    if ( pKeyState[SDL_SCANCODE_Q] || pKeyState[SDL_SCANCODE_ESCAPE]) 
    {
      bRunning = false;
    }
    else
    {
      if ( pKeyState[SDL_SCANCODE_LEFT] ) 
	cout << "arrow left pressed" << endl;

      if ( pKeyState[SDL_SCANCODE_RIGHT] ) 
	cout << "arrow right pressed" << endl;
    }
    
    // access mouse state 
    if ( SDL_GetMouseState(NULL,NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) )
    {
      cout << "Left button pressed" << endl;
    }
    
    int iMouseX, iMouseY;
    // access mouse state and get its coordinates while doing it.
    if ( SDL_GetMouseState(&iMouseX,&iMouseY) & SDL_BUTTON(SDL_BUTTON_RIGHT) )
    {
      cout << "Right button pressed at " 
	   << iMouseX << "," << iMouseY << endl;
    }
    // access relative mouse state 
    SDL_GetRelativeMouseState( &iMouseX, &iMouseY );
    cout << "relative mouse move  " 
	   << iMouseX << "," << iMouseY << endl;
    SDL_RenderPresent(renderer);
  }
  // cleanup
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(pScreen);
  SDL_Quit();
  return 0;
}
