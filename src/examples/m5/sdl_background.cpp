////////////////////////////////////////////////////////////////////////////////
/// SDL Programming
////////////////////////////////////////////////////////////////////////////////
/// Example of an multilevel scrolling effect.
/// \author anssi.grohn@karelia.fi
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <SDL_image.h>
#include <iostream> 
#include <stdexcept>

const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;

using namespace std;

enum BGELEMENT
{
  BG_SKY,
  BG_MOUNTAINS,
  BG_TREES_FAR,
  BG_TREES_NEAR,
  BG_NUM_BACKGROUNDS
};

struct BgElement
{
  SDL_Rect dest;
  SDL_Texture *pImage;
  int offset;
  int cum_steps;
  int step_limit;

  BgElement()
  {
    dest.x = 0;
    dest.y = 0;
    offset = 0;
    pImage = NULL;
    cum_steps = 0;
    step_limit = 1;
  }
  
  virtual ~BgElement()
  {
    SDL_DestroyTexture( pImage );
  }

  void Render( SDL_Window *window, SDL_Renderer *renderer )
  {
    SDL_Rect tmp = dest;
    tmp.x = dest.x;
    tmp.y = dest.y;
    int w;
    int h;
    SDL_QueryTexture(pImage, NULL, NULL, &w, &h);
    if ( offset > 0 )
    {
      offset -= w;
    }
    tmp.x += offset;
    
    int windowWidth;
    SDL_GetWindowSize(window, &windowWidth, NULL);
    while( tmp.x < windowWidth )
    {
      SDL_Rect blit = tmp;
      blit.w = w;
      blit.h = h;
      SDL_RenderCopy( renderer, pImage, NULL, &blit );
      tmp.x += w;
    }
    
  }
  
  void Move( int step )
  {
    int w;
    SDL_QueryTexture(pImage, NULL, NULL, &w, NULL);
    cum_steps += step;

    if ( cum_steps >= step_limit )
    {
      offset -= cum_steps/step_limit;
      cum_steps -= (cum_steps/step_limit)*step_limit;
    }
    else if ( cum_steps <= -step_limit )
    {
      offset += cum_steps/-step_limit;
      cum_steps += (cum_steps/step_limit)*-step_limit;
    }
    
    if ( offset >= w )
    {
      offset -= w * (offset / w);
    }
    else if ( offset <= -w )
    {
      offset += w * (offset / w);
    }
  }
};



int main( int argc, char **argv )
{

  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    throw runtime_error(SDL_GetError());

  SDL_Window *window = SDL_CreateWindow( "Scrolling background example",
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

  BgElement bg_array[BG_NUM_BACKGROUNDS];
  
  // Attempt to load image
  bg_array[BG_SKY].pImage         = IMG_LoadTexture(renderer, "./res/bg_level0.png");
  bg_array[BG_MOUNTAINS].pImage   = IMG_LoadTexture(renderer, "./res/bg_level1.png");
  bg_array[BG_MOUNTAINS].step_limit = 78;
  bg_array[BG_TREES_FAR].pImage   = IMG_LoadTexture(renderer, "./res/bg_level2.png");
  bg_array[BG_TREES_FAR].step_limit = 25;
  bg_array[BG_TREES_FAR].dest.y = 260;
  bg_array[BG_TREES_NEAR].pImage  = IMG_LoadTexture(renderer, "./res/bg_level3.png");
  bg_array[BG_TREES_FAR].step_limit = 2;  

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
      case SDL_KEYDOWN:
	switch( ev.key.keysym.sym )
	{
	case SDLK_LEFT:
	  bg_array[BG_TREES_NEAR].Move(-1);
	  bg_array[BG_TREES_FAR].Move(-1);
	  bg_array[BG_MOUNTAINS].Move(-1);
	  break;
	case SDLK_RIGHT:
	  bg_array[BG_TREES_NEAR].Move(1);
	  bg_array[BG_TREES_FAR].Move(1);	  
	  bg_array[BG_MOUNTAINS].Move(1);
	  break;
	default:
	  break;
	}
	break;
      }
    }
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear( renderer);
    
    bg_array[BG_SKY].Render(window, renderer);
    bg_array[BG_MOUNTAINS].Render(window,renderer);
    bg_array[BG_TREES_FAR].Render(window,renderer);
    bg_array[BG_TREES_NEAR].Render(window,renderer);

    // flip buffers
    SDL_RenderPresent( renderer );
  }



  SDL_Quit();


  return 0;
}


