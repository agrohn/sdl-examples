////////////////////////////////////////////////////////////////////////////////
/// SDL Programming
////////////////////////////////////////////////////////////////////////////////
/// Example of a pixel-based collision detection using two plane objects.
/// When collision is detected, a red rectangle appears.
///
/// \author anssi.grohn@karelia.fi (c) 2014.
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>
#include <SDL_image.h>
#include <iostream> 
#include <cassert>
#include <stdexcept>
////////////////////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////////////////////
const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;
////////////////////////////////////////////////////////////////////////////////
//
//
// +----------+
// |          |
// |  A   +---+--+
// |      | C |  |
// +------+---+  |
//        | B    |
//        +------+
////////////////////////////////////////////////////////////////////////////////

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}
////////////////////////////////////////////////////////////////////////////////
inline bool IsTransparent( SDL_Surface *pS, int x, int y )
{
  

  Uint32 pix = getpixel( pS, x,y);
  Uint32 colorkey;
  SDL_GetColorKey(pS, &colorkey);
  SDL_BlendMode mode;
  SDL_GetSurfaceBlendMode( pS,  &mode);

  if ( (pS->flags & SDL_TRUE) && 
       (pix == colorkey) ) 
  {
    return true;
  }
  else if ( (mode == SDL_BLENDMODE_BLEND) &&
            ( (pix & pS->format->Amask) == 0) )
  {
    return true;
  }
  
  return false;
}
////////////////////////////////////////////////////////////////////////////////
bool Collides( SDL_Surface *pA, int ax, int ay,
	       SDL_Surface *pB, int bx, int by,
	       SDL_Rect & rect )
{

  // get possibly intersecting rectangle in world coordinates
  //SDL_Rect coll;
  int cx, cy, cw, ch;
  if ( ax < bx )
  {
    cx = bx;
    cw = ax + pA->w - bx;   
  }
  else
  {
    cx = ax;
    cw = (bx + pB->w) - ax;
  }
  
  if ( ay < by )
  {
    cy = by;
    ch = ay + pA->h - by;
  }
  else
  {
    cy = ay;
    ch = by + pB->h - ay;      
  }

  SDL_LockSurface( pA );
  SDL_LockSurface( pB );

  for( int y = cy; y < cy+ch ; y++)
  {
    for( int x = cx; x < cx+ cw ; x++)
    {
      // If either of the pixels is transparent, then there is no collision
      if ( IsTransparent( pA, x-ax, y-ay) ) continue;
      if ( IsTransparent( pB, x-bx, y-by) ) continue;

      // For displaying collision evaluation area
      rect.x = cx;
      rect.y = cy;
      rect.w = cw;
      rect.h = ch;
      SDL_UnlockSurface( pA );
      SDL_UnlockSurface( pB );
      return true;
    }
  }
  SDL_UnlockSurface( pA );
  SDL_UnlockSurface( pB );
  return false;
}

int main( int argc, char **argv )
{

  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    throw runtime_error(SDL_GetError());
  
  SDL_Window *window = SDL_CreateWindow( "Per-pixel collision example",
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




  

  int bRunning = true;

  // Load images
  SDL_Surface *pNormal = IMG_Load("res/plane.bmp" );
  SDL_Surface *pGreen  = IMG_Load("res/plane_green.bmp" );

  if ( !pNormal  ) throw runtime_error( IMG_GetError());
  if ( !pGreen )   throw runtime_error( IMG_GetError());


  // Set colorkeys
  SDL_SetColorKey( pNormal, SDL_TRUE, SDL_MapRGB(pNormal->format, 0, 67,171 ));
  SDL_SetColorKey( pGreen,  SDL_TRUE,  SDL_MapRGB(pGreen->format, 0, 67,171 ));
  SDL_Rect normal_dest = { 294,234, pNormal->w,pNormal->h};
  SDL_Rect green_dest = {300, 240,pGreen->w,pNormal->h};

  SDL_Texture *normalTex = SDL_CreateTextureFromSurface(renderer, pNormal);
  SDL_Texture *greenTex = SDL_CreateTextureFromSurface(renderer, pGreen);
  if ( !normalTex )  throw runtime_error( SDL_GetError());
  if ( !greenTex )  throw runtime_error( SDL_GetError());
  const int STEP = 3;
  SDL_Rect tmp;
  
  bool collides = false;
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
	case SDLK_DOWN:
	  normal_dest.y += STEP;
	 
	  break;
	case SDLK_UP:
	  normal_dest.y -= STEP;
	
	  break;
	case SDLK_RIGHT:
	  normal_dest.x += STEP;
	 
	  break;
	case SDLK_LEFT:
	  normal_dest.x -= STEP;
	 
	  break;
	default:
	  break;
	}
	collides = Collides( pGreen, green_dest.x, green_dest.y, 
			     pNormal, normal_dest.x, normal_dest.y, tmp);
	break;
      }
      
      SDL_SetRenderDrawColor(renderer,0,0,0,0);
      SDL_RenderClear(renderer);
      

      if ( collides )
      {
	SDL_SetRenderDrawColor(renderer,255,0,0,255);
	SDL_RenderFillRect( renderer, &tmp);
      }
      
      SDL_RenderCopy( renderer, normalTex, NULL, &normal_dest);
      SDL_RenderCopy( renderer, greenTex, NULL,  &green_dest);
      SDL_RenderPresent( renderer );
      SDL_Delay(3);
    }
  }
  return 0;
}
