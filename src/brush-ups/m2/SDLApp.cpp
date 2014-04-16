/////////////////////////////////////////////////////////////////
/// SDL Programming
////////////////////////////////////////////////////////////////////////////////
/// Handler class for SDL stuff.
/// \author anssi.grohn@karelia.fi (c) 2014
/////////////////////////////////////////////////////////////////
#include <SDLApp.h>
#include <stdexcept>
////////////////////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////////////////////
SDLApp::SDLApp()
{
  // initialize SDL
  if ( SDL_Init( SDL_INIT_EVERYTHING) < 0 )
    throw runtime_error(SDL_GetError());
  // no dangling pointers
  renderer_ = NULL;
  window_ = NULL;
}
////////////////////////////////////////////////////////////////////////////////
void 
SDLApp::Init()
{
  // create window
  window_ = SDL_CreateWindow("Hello", 
			     SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
			     800, 600, SDL_WINDOW_SHOWN);
  if ( window_ == NULL ) throw runtime_error(SDL_GetError());

  // create renderer
  renderer_ = SDL_CreateRenderer( window_, -1, SDL_RENDERER_SOFTWARE);
  if ( renderer_ == NULL ) throw runtime_error(SDL_GetError());
}
////////////////////////////////////////////////////////////////////////////////
SDLApp::~SDLApp()
{
  // destroy renderer & window
  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
  // deinitialize SDL
  SDL_Quit();
}
////////////////////////////////////////////////////////////////////////////////
void
SDLApp::Render()
{
  // our simple render function.
  SDL_RenderPresent(renderer_);
}
////////////////////////////////////////////////////////////////////////////////
void 
SDLApp::HandleInput()
{
  // poll keydown and quit, call appropriate handler afterwards.
  SDL_Event ev;
  while( SDL_PollEvent(&ev))
  {
    switch( ev.type )
    {
    case SDL_KEYDOWN:
      OnKeyPress(ev.key.keysym.sym); 
      break;
    case SDL_QUIT:
      OnQuit();
      break;
    }
  }
}
////////////////////////////////////////////////////////////////////////////////
