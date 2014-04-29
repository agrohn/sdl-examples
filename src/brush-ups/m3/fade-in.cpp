////////////////////////////////////////////////////////////////////////////////
// SDL Programming
////////////////////////////////////////////////////////////////////////////////
// Fade-in-out brush-up answer.
// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <iostream> 
#include <stdexcept>

using namespace std;

const int WINDOW_WIDTH  = 1024;
const int WINDOW_HEIGHT = 566;

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
  
  SDL_Window *window = SDL_CreateWindow( "Fade-in-out",
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
  SDL_Texture * pImage = IMG_LoadTexture(renderer, "res/MohammedAgbadi_HEROART.jpg");
  if ( pImage == NULL) throw runtime_error(IMG_GetError());
  
  
  
  SDL_Event ev;
  Uint32 time = SDL_GetTicks();
  int alpha = 255;
  bool bRunning = true;
  float totalTime = 0.0f;

  // State for our application
  enum State { FadeIn, FadeOut, Idle };
  
  // By default, state is idle = no fading
  State fadeState = Idle;

  while ( bRunning )
  {
    while(SDL_PollEvent(&ev))
    {
      switch(ev.type){
      case SDL_QUIT:
	bRunning = false;
	break;
	// when mouse button is pressed, fading starts (if idle)
      case SDL_MOUSEBUTTONDOWN:
	if ( fadeState == Idle)
	  fadeState = FadeIn;
	break;
      default:
	break;
      }
    }
    // clear screen to black.
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear(renderer);
    SDL_RenderCopy( renderer, pImage, NULL, NULL);

    // Draw black box over everything with desired alpha value
    boxRGBA( renderer, 0,0, WINDOW_WIDTH, WINDOW_HEIGHT, 0,0,0, (Uint8)alpha);

    float seconds = (float)(SDL_GetTicks()-time)*0.001f;
    totalTime += seconds;

    const int ALPHA_STEP_PER_SECOND = 50;
    // Work only when some time has actually passed
    if ( totalTime > 0.05f )
    {
      // reset time reference point
      time = SDL_GetTicks();

      if ( fadeState == FadeIn )
      {
	alpha -= totalTime * ALPHA_STEP_PER_SECOND;  
	// start fading out once done
	if ( alpha <= 0 ) 
	{
	  fadeState = FadeOut;
	  alpha = 0;
	}
      }
      else if ( fadeState == FadeOut )
      {
	// increase alpha gradually
	alpha += totalTime * ALPHA_STEP_PER_SECOND;
	
	// once completely transparent, set mode to idle.
	if ( alpha > 255 )
	{
	  alpha = 255;
	  fadeState = Idle;
	}
	
      }
      totalTime = 0.0f;
    }
    // add some delay to main loop
    SDL_Delay(5);
    SDL_RenderPresent( renderer );
  }

  SDL_DestroyTexture(pImage);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();
  return 0;
}
