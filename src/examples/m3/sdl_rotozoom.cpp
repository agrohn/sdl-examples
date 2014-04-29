////////////////////////////////////////////////////////////////////////////////
// SDL Programming 
////////////////////////////////////////////////////////////////////////////////
// Example of rotation and scaling using SDL_gfx.
// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <SDL_image.h>
#include <SDL2_rotozoom.h>
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

  // Load images
  SDL_Surface * pImage  = IMG_Load("./res/plane.png");
  float fRotation = 0.0f;
  float fScale = 0.1f;

  if ( pImage == NULL  )
    throw runtime_error(IMG_GetError());
  


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
      
    SDL_Rect dest;
    dest.x = 220;
    dest.y = 220;  

    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);

    SDL_Surface *pTmp = rotozoomSurface( pImage, fRotation, fScale, 1 );
    dest.x -= pTmp->w / 2;
    dest.y -= pTmp->h / 2;
    dest.w  = pTmp->w;
    dest.h  = pTmp->h;
    SDL_Texture *pTexture = SDL_CreateTextureFromSurface(renderer,pTmp);

    SDL_RenderCopy( renderer, pTexture, NULL, &dest);
      
    SDL_FreeSurface( pTmp );
    SDL_DestroyTexture(pTexture);

    SDL_RenderPresent( renderer );
 
    // Append rotation and scale
    fRotation += 0.5f;
    if ( fRotation > 360.0f ) fRotation -= 360.0f;
    fScale+=0.001f;
    if ( fScale > 3.0f ) fScale = 0.1f;
  }

  SDL_FreeSurface(pImage );
  SDL_Quit();
  
  return 0;
}
