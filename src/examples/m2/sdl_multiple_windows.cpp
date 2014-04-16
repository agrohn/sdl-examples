////////////////////////////////////////////////////////////////////////////////
// SDL Programming
////////////////////////////////////////////////////////////////////////////////
// Program that opens two SDL graphics windows, creates two separate renderers,
// draws different images to them and waits for Q to be pressed.
// 
// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <SDL_image.h>
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
  
  // Initialize SDL_image library (load JPGs and PNGs)
  int initFlags = IMG_INIT_JPG | IMG_INIT_PNG;
  int retFlags  = IMG_Init( initFlags );

  if ( retFlags != initFlags )
  {
    throw runtime_error(IMG_GetError());
  }
  
  SDL_Window *window1 = SDL_CreateWindow( "Window 1",

					  SDL_WINDOWPOS_CENTERED,
					  SDL_WINDOWPOS_CENTERED,
					  WINDOW_WIDTH, 
					  WINDOW_HEIGHT, 
					  SDL_WINDOW_SHOWN
					  );
  if ( window1 == NULL )  throw runtime_error(SDL_GetError());
  

  SDL_Window *window2 = SDL_CreateWindow( "Window 2",
					  SDL_WINDOWPOS_CENTERED,
					  SDL_WINDOWPOS_CENTERED,
					  WINDOW_WIDTH, 
					  WINDOW_HEIGHT, 
					  SDL_WINDOW_SHOWN
					  );

  if ( window2 == NULL )    throw runtime_error(SDL_GetError());


  SDL_Renderer *renderer1 = SDL_CreateRenderer( window1, -1, SDL_RENDERER_SOFTWARE);
  SDL_Renderer *renderer2 = SDL_CreateRenderer( window2, -1, SDL_RENDERER_SOFTWARE);

  // Load image
  SDL_Texture * pImage = IMG_LoadTexture(renderer1, "res/plane.png");
  if ( pImage == NULL) throw runtime_error(IMG_GetError());
  int w,h;
  SDL_QueryTexture(pImage, NULL, NULL, &w, &h);
  

  SDL_Texture * pLogo = IMG_LoadTexture(renderer2, "res/sdl-logo.png");
  if ( pLogo == NULL) throw runtime_error(IMG_GetError());

  SDL_SetRenderDrawColor(renderer1, 0,0,0,0);
  SDL_SetRenderDrawColor(renderer2, 0,0,0,0);

  SDL_Rect dest;

  dest.x = 120;
  dest.y = 220;  
  dest.w = w;
  dest.h = h;
  bool bRunning = true;
  while ( bRunning )
  {
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
      switch(e.type)
      {
      case SDL_QUIT:
	bRunning = false;
	break;
      case SDL_KEYDOWN:
	if ( e.key.keysym.sym == SDLK_q)
	{
	  bRunning = false;
	}
      default:
	break;
      }
    }
    // clear screen to black.
    SDL_SetRenderDrawColor(renderer1, 0,0,0,0);
    SDL_RenderClear(renderer1);
  
    // Draw image on screen, with alpha blending. 
    // Background color shows through.
    SDL_SetTextureAlphaMod( pImage, 255);
    SDL_SetTextureBlendMode( pImage, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy( renderer1, pImage, NULL, &dest);

  
    SDL_RenderCopy(renderer2, pLogo, NULL, NULL);

  
    SDL_RenderPresent( renderer1 );
    SDL_RenderPresent( renderer2 );

  
  }

  SDL_DestroyRenderer(renderer1);
  SDL_DestroyRenderer(renderer2);

  SDL_DestroyWindow(window1);
  SDL_DestroyWindow(window2);

  IMG_Quit();
  SDL_Quit();
  return 0;
}
