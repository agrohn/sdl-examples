////////////////////////////////////////////////////////////////////////////////
// SDL Programming
////////////////////////////////////////////////////////////////////////////////
// Example of string rendering using truetype fonts.
// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>
#include <iostream> 
#include <stdexcept>

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

  SDL_Renderer *renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_SOFTWARE);

  if ( renderer == NULL )
    throw runtime_error(SDL_GetError());



  /////////////////////////////////////////////////////////////////
  // Font loading 
  TTF_Font *pFont = TTF_OpenFont("res/BNJinx.ttf", 28);
  
  // In case there were problems...
  if ( pFont == NULL )
  {
    throw runtime_error(TTF_GetError());
  }
  /////////////////////////////////////////////////////////////////
  SDL_Color white = { 255, 255, 255, 255 };
  SDL_Color blue = { 0, 0, 255 };

  enum SURFACES { SOLID, SHADED, BLENDED, NUM_SURFACES };
  SDL_Surface* pText[NUM_SURFACES];
  // Render surfaces with SDL_ttf first
  pText[SOLID] = TTF_RenderText_Solid(pFont, 
				      "This text is solid.",
				      white);

  pText[SHADED] = TTF_RenderText_Shaded(pFont, "This one is shaded with its own background color",
					white, blue);

  pText[BLENDED] = TTF_RenderText_Blended(pFont, "This one looks smooth with any background",
					  white);

  SDL_Texture* pTextures[NUM_SURFACES];

  // Convert surfaces to textures
  for(int i=0;i<NUM_SURFACES;i++) {

    pTextures[i] = SDL_CreateTextureFromSurface(renderer, pText[i]);
    // free surface,  not needed anymore.
    SDL_FreeSurface(pText[i]);
  }
  
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
      }
    }
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);
    // Fill half of the screen with orange box
    boxRGBA(renderer, 0,0, WINDOW_WIDTH*0.5, WINDOW_HEIGHT, 255,128,0,255);

    // Render textures and get proper texture dimensions while doing it

    // Render solid texture first...
    SDL_Rect dest = {220, 120, 0,0};
    SDL_QueryTexture(pTextures[SOLID], NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, pTextures[SOLID], NULL, &dest);
    
    // ..then shaded one...
    dest.x = 20;
    dest.y = 220;
    SDL_QueryTexture(pTextures[SHADED], NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, pTextures[SHADED], NULL, &dest);

    // .. and lastly, blended one...
    dest.y = 320;
    SDL_QueryTexture(pTextures[BLENDED], NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, pTextures[BLENDED], NULL, &dest);

    SDL_RenderPresent( renderer );
  }
  // Cleanup
  TTF_CloseFont(pFont);
  TTF_Quit();
  SDL_Quit();
  return 0;
}
