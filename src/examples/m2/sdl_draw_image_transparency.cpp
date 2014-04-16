////////////////////////////////////////////////////////////////////////////////
// SDL Programming
////////////////////////////////////////////////////////////////////////////////
// Program that opens SDL graphics window, loads an images of a aeroplanes
// and renders them to screen normally and using alpha bleding. 
// Application exits in three seconds after loading.
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
  
  SDL_Window *window = SDL_CreateWindow( "Hello",
					 SDL_WINDOWPOS_CENTERED,
					 SDL_WINDOWPOS_CENTERED,
					 WINDOW_WIDTH, 
					 WINDOW_HEIGHT, 
					 SDL_WINDOW_SHOWN
					 );
  if ( window == NULL )
  {
    throw runtime_error(SDL_GetError());
  }

  SDL_Renderer *renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);


  // Load image
  SDL_Texture * pImage = IMG_LoadTexture(renderer, "res/plane.png");
  if ( pImage == NULL) throw runtime_error(IMG_GetError());
  int w,h;
  SDL_QueryTexture(pImage, NULL, NULL, &w, &h);
  
  SDL_SetRenderDrawColor(renderer, 0,0,0,0);
  

  SDL_Rect dest;

  dest.x = 120;
  dest.y = 220;  
  dest.w = w;
  dest.h =h;
  // clear screen to black.
  SDL_SetRenderDrawColor(renderer, 0,0,0,0);
  SDL_RenderClear(renderer);
  
  // Draw image on screen, with alpha blending. 
  // Background color shows through.
  SDL_SetTextureAlphaMod( pImage, 255);
  SDL_SetTextureBlendMode( pImage, SDL_BLENDMODE_BLEND);
  SDL_RenderCopy( renderer, pImage, NULL, &dest);
  SDL_Rect tmp = dest;

  tmp.x += dest.w/2+dest.w/5;
  // Draw image on screen, without alpha blending - covers entire area, 
  // no background color shows through.
  SDL_SetTextureBlendMode( pImage, SDL_BLENDMODE_NONE);

  SDL_RenderCopy( renderer, pImage, NULL, &tmp);
  tmp.x += dest.w/2+dest.w/5;
  // Draw image on screen, without alpha blending
  SDL_SetTextureAlphaMod( pImage, 128);
  SDL_SetTextureBlendMode( pImage, SDL_BLENDMODE_BLEND);
  SDL_RenderCopy( renderer, pImage, NULL, &tmp);
  
  SDL_RenderPresent( renderer );



  SDL_Delay(5000);

  IMG_Quit();
  SDL_Quit();
  return 0;
}
