////////////////////////////////////////////////////////////////////////////////
// SDL Programming
////////////////////////////////////////////////////////////////////////////////
// Example of how to utilize Page class.
// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <SDL_ttf.h>
#include <iostream> 
#include <stdexcept>
#include <Page.h>
using namespace std;

const int WINDOW_WIDTH  = 800;
const int WINDOW_HEIGHT = 600;

int main( int argc, char **argv )
{
  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
  {
    throw runtime_error(SDL_GetError());
  }
  // Initialize SDL_ttf library
  if ( TTF_Init() < 0 )
    throw runtime_error(TTF_GetError());
 

  
  SDL_Window *window = SDL_CreateWindow( "SDL_ttf example",
					 SDL_WINDOWPOS_CENTERED,
					 SDL_WINDOWPOS_CENTERED,
					 WINDOW_WIDTH, 
					 WINDOW_HEIGHT, 
					 SDL_WINDOW_SHOWN
					 );
  
  if ( window == NULL )
    throw runtime_error(SDL_GetError());

  SDL_Renderer *renderer = SDL_CreateRenderer( window, -1, 
					       SDL_RENDERER_ACCELERATED |   // to make font rendering work properly (bug?)
					       SDL_RENDERER_TARGETTEXTURE); // To enable rendering to targets

  if ( renderer == NULL )
    throw runtime_error(SDL_GetError());



  /////////////////////////////////////////////////////////////////
  // Font loading 
  TTF_Font *pFont = TTF_OpenFont("res/BNJinx.ttf", 22);
  
  // In case there were problems...
  if ( pFont == NULL )
  {
    throw runtime_error(TTF_GetError());
  }
  /////////////////////////////////////////////////////////////////

  bool bRunning = true;
  // Create pagse
  Page pageA;
  Page pageB;
  // Initialize with custom texture size
  pageA.Init( renderer, 400,550); 
  // Put content on page
  pageA << "You can use this thing just like cout. " << 2 << "x brilliant\n";
  // set justification
  pageA._justification = Justification::Left;


  // Initialize with custom texture size
  pageB.Init( renderer, 400,550); 
  // Put content on page
  pageB << "this one works as well and has different justification\n.";
  // set justification
  pageB._justification = Justification::Right;

  // compute lines 
  pageA.Compose(pFont);
  pageB.Compose(pFont);
  // Render to internal texture (_pageTexture)
  pageA.RenderContent(renderer);
  pageB.RenderContent(renderer);
  
  // update requirement can be flagged and queried with dirty (does not affect functionality as such).
  //pageB.SetDirty(true);
  //pageB.IsDirty();

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
    SDL_Rect tmp = {0,0,400,550};
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderClear(renderer);
    // Render textures from pages to screen
    SDL_RenderCopy(renderer, pageA._pageTexture, NULL, &tmp);
    tmp.x = 400;
    SDL_RenderCopy(renderer, pageB._pageTexture, NULL, &tmp);
    
    SDL_RenderPresent( renderer );
  }
  // Cleanup
  TTF_CloseFont(pFont);
  TTF_Quit();
  SDL_Quit();
  return 0;
}
