///////////////////////////////////////////////////////////////////////////////
// SDL Programming
////////////////////////////////////////////////////////////////////////////////
// Example code which limits frame rate.
// Uses SDL_ttf to display changes in FPS to screen.
// Use keys 1, up arrow and down arrow to control drawing.
// Key 1 sets repeat to 1.
// Arrow up doubles repeat.
// Arrow down halves repeat.
// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <SDL2_gfxPrimitives.h>
#include <SDL2_framerate.h>
// header for truetype stuff
#include <SDL_ttf.h>
#include <iostream> 
#include <stdexcept>
#include <sstream>

const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;


using namespace std;

int main( int argc, char **argv )
{
  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
  {
    throw runtime_error(SDL_GetError());
  }
  // initialize
  if (TTF_Init() < 0)
  {
    throw runtime_error(TTF_GetError());
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
  

  // Open font
  TTF_Font * font = TTF_OpenFont("res/LobsterTwo-Regular.ttf", 24);
  if ( font == NULL )
    throw runtime_error(TTF_GetError());

  
  FPSmanager fpsManager;
  SDL_initFramerate( &fpsManager );
  SDL_setFramerate( &fpsManager, 60);
  bool bRunning = true;

  int frameCount = 0;
  Uint32 time = SDL_GetTicks();
  int repeat = 1;

  // Render GUI texts
  SDL_Color col = { 255,255,255,255 };

  SDL_Surface * tmp = TTF_RenderText_Blended( font, "Hit arrow up and down to double / halve amount of rendered rectangles", col);
  SDL_Texture *guiHelp = SDL_CreateTextureFromSurface( renderer, tmp);
  SDL_FreeSurface(tmp);
  SDL_Rect guiHelpRect = { 0,0};
  SDL_QueryTexture( guiHelp, NULL, NULL, &guiHelpRect.w, &guiHelpRect.h );
  
  // texture pointer for holding fps text (changing)
  SDL_Texture *fpsText = NULL;  
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
	  if ( repeat == 0 ) repeat = 1;
	  else repeat *= 2;
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
    SDL_RenderClear(renderer);
    for( int i=0;i<repeat;i++)
      boxRGBA(renderer, 0,0,WINDOW_WIDTH,WINDOW_HEIGHT, 255,0,0,255);
    
    // Delay drawing loop
    SDL_framerateDelay(&fpsManager);
    
    // Determine when one second has passed
    if ( SDL_GetTicks() - time > 1000 )
    {
      frameCount = 0;
      time = SDL_GetTicks();
    }
    else 
      ++frameCount;

    
    SDL_RenderCopy(renderer, guiHelp, NULL, &guiHelpRect);

    stringstream ss;
    int fps = (int)(frameCount/((float)(SDL_GetTicks()-time)*0.001f));
    ss << "FPS is : " << fps << " drawing " << repeat << " rectangles." << endl;
    frameCount = 0;
    time = SDL_GetTicks();

    SDL_Surface * pText = TTF_RenderText_Blended( font, ss.str().c_str(), col);
    
    if ( fpsText ) SDL_DestroyTexture(fpsText);
    fpsText = SDL_CreateTextureFromSurface(renderer,pText);
    SDL_FreeSurface(pText);

    if ( fpsText ) 
    {
      SDL_Rect dest = {0,WINDOW_HEIGHT/2};
      SDL_QueryTexture( fpsText, NULL, NULL, &dest.w, &dest.h);
      SDL_RenderCopy(renderer, fpsText, NULL, &dest) ;
    }
    // Put stuff on screen again
    SDL_RenderPresent( renderer );
    SDL_Delay(1);
  }
  TTF_Quit();
  SDL_DestroyTexture(fpsText);
  SDL_DestroyTexture(guiHelp);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
