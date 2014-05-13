////////////////////////////////////////////////////////////////////////////////
/// SDL Programming
////////////////////////////////////////////////////////////////////////////////
/// Example of an multilevel scrolling effect in both vertical and horizontal direction.
/// \author anssi.grohn@karelia.fi (c) 2014
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
  BG_STARS,
  BG_STARS2,
  BG_STARS3,
  BG_NUM_BACKGROUNDS
};

enum DIR
{
  HORIZ,
  VERT,
  NUM_DIRS
};

struct BgElement
{
  SDL_Rect dest;
  SDL_Texture *pImage;
  int offset[NUM_DIRS];
  int cum_steps[NUM_DIRS];
  int step_limit;

  BgElement()
  {
    dest.x = 0;
    dest.y = 0;
    offset[0] = offset[1] = 0;
    pImage = NULL;
    cum_steps[0] = cum_steps[1] = 0;
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
    if ( offset[VERT] > 0 )
    {
      offset[VERT] -= h;
    }
    tmp.y += offset[VERT];
    int windowWidth;
    SDL_GetWindowSize(window, &windowWidth, NULL);

    while( tmp.y < h )
    {
      if ( offset[HORIZ] > 0 )
      {
      	offset[HORIZ] -= w;
      }
      tmp.x += offset[HORIZ];
      while( tmp.x < windowWidth )
      {
	SDL_Rect blit = tmp;
	blit.w = w;
	blit.h = h;
	SDL_RenderCopy( renderer, pImage, NULL, &blit );
	tmp.x += w;
      }

      tmp.y += h;
      tmp.x = dest.x;
    }
  }
  
  void Move( DIR d, int step )
  {
    int w,h;
    SDL_QueryTexture(pImage, NULL, NULL, &w, &h);
    cum_steps[d] += step;
    if ( cum_steps[d] >= step_limit )
    {
      offset[d] -= cum_steps[d]/step_limit;
      cum_steps[d] -= (cum_steps[d]/step_limit)*step_limit;
    }
    else if ( cum_steps[d] <= -step_limit )
    {
      offset[d] += cum_steps[d]/-step_limit;
      cum_steps[d] -= (cum_steps[d]/-step_limit)*-step_limit;
    }    
    
    int steps_h = offset[HORIZ] / w;
    int steps_v = offset[VERT] / h;

    if ( offset[HORIZ] >= w )       offset[HORIZ] += w * steps_h;
    else if ( offset[HORIZ] <= -w ) offset[HORIZ] -= w * steps_h;

    if ( offset[VERT] >= h )       offset[VERT] += h * steps_v;
    else if ( offset[VERT] <= -h ) offset[VERT] -= h * steps_v;
  }
};

int main( int argc, char **argv )
{
  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    throw runtime_error(SDL_GetError());

  SDL_Window *window = SDL_CreateWindow( "Scrolling background 2 example",
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
  bg_array[BG_STARS].pImage  = IMG_LoadTexture(renderer, "./res/stars2.png");
  bg_array[BG_STARS].step_limit += 20;
  bg_array[BG_STARS2].pImage = IMG_LoadTexture(renderer,"./res/stars.png");
  bg_array[BG_STARS2].step_limit += 12;
  bg_array[BG_STARS3].pImage = IMG_LoadTexture(renderer,"./res/stars3.png");
  bg_array[BG_STARS3].step_limit += 2;
  bool bRunning = true;
  
  int step = 6;
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
	  bg_array[BG_STARS].Move(HORIZ,-step);
	  bg_array[BG_STARS2].Move(HORIZ,-step);
	  bg_array[BG_STARS3].Move(HORIZ,-step);
	  break;
	case SDLK_RIGHT:
	  bg_array[BG_STARS].Move(HORIZ,step);
	  bg_array[BG_STARS2].Move(HORIZ,step);
	  bg_array[BG_STARS3].Move(HORIZ,step);
	  break;
	case SDLK_UP:
	  bg_array[BG_STARS].Move(VERT,-step);
	  bg_array[BG_STARS2].Move(VERT,-step);
	  bg_array[BG_STARS3].Move(VERT,-step);
	  break;
	case SDLK_DOWN:
	  bg_array[BG_STARS].Move(VERT,step);
	  bg_array[BG_STARS2].Move(VERT,step);
	  bg_array[BG_STARS3].Move(VERT,step);
	  break;
	default:
	  break;
	}
	break;
      }
    }
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear( renderer);

    bg_array[BG_STARS].Render(window, renderer);
    bg_array[BG_STARS2].Render(window, renderer);
    bg_array[BG_STARS3].Render(window, renderer);

    // flip buffers
    SDL_RenderPresent( renderer );
  }

  SDL_Quit();

  return 0;
}

