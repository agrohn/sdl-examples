////////////////////////////////////////////////////////////////////////////////
/// SDL Programming 
////////////////////////////////////////////////////////////////////////////////
/// Example of a tile-based graphics made manually.
/// \author anssi.grohn@karelia.fi (c) 2010-2015
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <SDL_image.h>
#include <iostream> 
#include <stdexcept>
////////////////////////////////////////////////////////////////////////////////
const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;
const int COLOR_DEPTH_IN_BITS = 32;
////////////////////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////////////////////
void SetRect( SDL_Rect & r, int x, int y, int w, int h )
{
  r.x = x;
  r.y = y;
  r.w = w;
  r.h = h;
}
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv )
{

  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    throw runtime_error(SDL_GetError());

  SDL_Window *window = SDL_CreateWindow( "Rectangle collision example",
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


  // Load tile data, this time as a surface.
  SDL_Surface *pTileSurface = IMG_Load("res/tile_bg.png");
  if ( pTileSurface == NULL )
  {
    throw std::runtime_error(IMG_GetError());
  }
  
  // Set color key information for surface.
  SDL_SetColorKey( pTileSurface,
		   SDL_TRUE,
		   SDL_MapRGB( pTileSurface->format, 239, 107, 8 ));

  // Convert color keyed surface as texture
  SDL_Texture *pBackgroundTiles = SDL_CreateTextureFromSurface(renderer, pTileSurface);
  if ( pBackgroundTiles == NULL )
  {
    throw std::runtime_error(SDL_GetError());
  }
  
  // Remove unneeded surface
  SDL_FreeSurface( pTileSurface);
  // symbolic constants for readability
  enum BGELEM  { GRASS, MUD, BUSH, CHOPPER, TREE, NUM_ELEMENTS };

  const int HEIGHTBLOCKS = 10;
  const int WIDTHBLOCKS  = 14;
  const int TILE_SIZE    = 48;

  SDL_Rect bg[NUM_ELEMENTS];
  
  SetRect( bg[GRASS],    96,  144, TILE_SIZE, TILE_SIZE );
  SetRect( bg[MUD],      143, 144, TILE_SIZE, TILE_SIZE );
  SetRect( bg[BUSH],     400, 192, TILE_SIZE, TILE_SIZE );
  SetRect( bg[CHOPPER],  355, 222, 44, 44 );
  SetRect( bg[TREE],     358, 134, 58, 56 );

  int tile_map[HEIGHTBLOCKS][WIDTHBLOCKS] = { 

    { GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS },
    { GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS },
    { GRASS, GRASS, MUD,   GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS },
    { GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS },
    { GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS },
    { GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS },
    { GRASS, GRASS, GRASS, GRASS, MUD,   GRASS, GRASS, GRASS, MUD,   GRASS },
    { GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS },
    { GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS },
    { GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS }

  };
  
  int tile_map_extra[HEIGHTBLOCKS][WIDTHBLOCKS] = { 

    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 4, 4, 0, 0, 4, 4, 0, 0, 0, 2, 2, 0 },
    { 0, 4, 0, 0, 4, 0, 4, 0, 4, 0, 0, 2, 2, 0 },
    { 0, 0, 0, 0, 4, 0, 4, 0, 0, 4, 0, 2, 2, 0 },
    { 3, 0, 0, 0, 4, 0, 4, 0, 0, 4, 0, 2, 2, 0 },
    { 0, 0, 0, 4, 0, 0, 4, 3, 0, 4, 0, 2, 2, 0 },
    { 0, 0, 4, 0, 0, 0, 4, 0, 0, 4, 0, 0, 0, 0 },
    { 0, 4, 0, 0, 0, 0, 4, 0, 4, 0, 0, 2, 2, 0 },
    { 0, 4, 4, 4, 4, 0, 4, 4, 0, 0, 0, 2, 2, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  };

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
    SDL_RenderCopy( renderer, pBackgroundTiles, NULL, NULL);
    // Draw underlying stuff first
    for( int r=0;r<HEIGHTBLOCKS;r++)
    {
      for( int c=0;c<WIDTHBLOCKS;c++)
      {
	// compute dest rect and render.
	SDL_Rect dest = { c * TILE_SIZE, r * TILE_SIZE, TILE_SIZE, TILE_SIZE };
	SDL_RenderCopy( renderer, pBackgroundTiles, &bg[tile_map[r][c]], &dest);
      } 
    }
    // Draw overlaying stuff
    for( int r=0;r<HEIGHTBLOCKS;r++)
    {
      for( int c=0;c<WIDTHBLOCKS;c++)
      {
	int tile = tile_map_extra[r][c];
	if ( tile != 0)
	{
	  // take care of special dimensions in two tiles.
	  int w,h;
	  w = h = TILE_SIZE;
	  switch(tile)
	  {
	  case CHOPPER:
	    w = h = 44;
	    break;
	  case TREE:
	    w = 58;
	    h = 56;
	    break;
	  }
	  // compute dest rect and render.
	  SDL_Rect dest = { c * TILE_SIZE, r * TILE_SIZE, w,h };
	  SDL_RenderCopy( renderer, pBackgroundTiles, &bg[tile_map_extra[r][c]], &dest);
	}
      } 
    }
    // Changes to screen
    SDL_RenderPresent( renderer );
    SDL_Delay(1);
  }
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}
