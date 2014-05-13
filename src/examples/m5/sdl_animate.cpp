////////////////////////////////////////////////////////////////////////////////
/// SDL Programming
////////////////////////////////////////////////////////////////////////////////
/// Example of simple sprite animation.
/// \author anssi.grohn@karelia.fi
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream> 
#include <stdexcept>

const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;

enum ANIM_TYPE { SPEAK_RIGHT, SPEAK_SCREEN };
using namespace std;
////////////////////////////////////////////////////////////////////////////////
// Helpful function for setting proper coordinates
void
CreateAnimation( SDL_Rect *pArr, int animsize, int x, int y, int w, int h)
{
  for( int i=0;i<animsize; i++)
  {
    pArr[i].x = x + (i * w);
    pArr[i].y = y;
    pArr[i].w = w;
    pArr[i].h = h;
    //cout << pArr[i].x << " " << pArr[i].y << " " << pArr[i].w << " " << pArr[i].h << endl;
  }
}
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv )
{

  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    throw runtime_error(SDL_GetError());

  if ( TTF_Init() < 0 )
    throw runtime_error(TTF_GetError());
  
  TTF_Font *pFont = TTF_OpenFont("./res/efmi.ttf", 24);
  if ( pFont == NULL )
    throw runtime_error(TTF_GetError());

  SDL_Window *window = SDL_CreateWindow( "Animation example",
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
  SDL_Texture * pAnim = IMG_LoadTexture(renderer,"./res/Guybrush.png");
  if ( pAnim == NULL )
    throw runtime_error(SDL_GetError());

  
  // Rectangles 
  SDL_Rect anims[12];

  // Create rectangles that represent frames
  CreateAnimation( &(anims[0]), 6, 42,60, 34, 48 );
  CreateAnimation( &(anims[6]), 6, 42,5, 34, 48 );
  
  // Which rectangles from image will be displayed in which animation.
  // This array determines the animation length and the frame display order.

  int anim_sequence[][14] = { 
    {6, 7, 8, 9,10,11,10, 9, 8, 7, 6, 7, 8, 9}, // speaking facing right
    {0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 2, 3, 4, 5}  // speaking towards player
  }; 

  int frame=0;              // which frame will be displayed
  int type = SPEAK_RIGHT;   // which animation the frame refers to 
  
  SDL_Color white = { 255, 255, 255, 255 };
  // Something for the character to say
  SDL_Surface * pText= TTF_RenderText_Blended(pFont, "Wha-huh?! ",  white);
  SDL_Surface * pText2 = TTF_RenderText_Blended(pFont, "Okay, now write your own code!",  white);
  if ( pText == NULL )  throw runtime_error(SDL_GetError());
  if ( pText2 == NULL )  throw runtime_error(SDL_GetError());
  // Storage for textures
  SDL_Texture * textures[2];
  const int WHAHUH = 0;
  const int WRITEYOUROWN = 1;
  // Convert surfaces to textures
  textures[WHAHUH] = SDL_CreateTextureFromSurface(renderer, pText);
  if ( textures[WHAHUH] == NULL ) throw runtime_error(SDL_GetError());
  textures[WRITEYOUROWN] = SDL_CreateTextureFromSurface(renderer, pText2);
  if ( textures[WRITEYOUROWN] == NULL ) throw runtime_error(SDL_GetError());

  // Release surfaces
  SDL_FreeSurface(pText);
  SDL_FreeSurface(pText2);
  
  bool bAnimating = true;
  bool bRunning = true;

  SDL_Rect dest; // animation placement
  dest.x = 220;
  dest.y = 220;  
 
  SDL_Rect destf; // font placement
  destf.x = 220;
  destf.y = 120;


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

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear( renderer);

    dest.w = anims[anim_sequence[type][frame]].w*2;
    dest.h = anims[anim_sequence[type][frame]].h*2;
    // Draw image on screen
    SDL_RenderCopy( renderer, pAnim, &anims[anim_sequence[type][frame]], &dest);
      
    if ( bAnimating ) frame++;

    if ( type == SPEAK_RIGHT )
    {
      // get text dimensions
      SDL_QueryTexture(textures[WHAHUH], NULL, NULL, &destf.w, &destf.h);
      // Draw image on screen
      SDL_RenderCopy( renderer, textures[WHAHUH], NULL, &destf);
    }
    else if ( type == SPEAK_SCREEN && bAnimating )
    {
      // get text dimensions
      SDL_QueryTexture(textures[WRITEYOUROWN], NULL, NULL, &destf.w, &destf.h);
      // Draw image on screen
      SDL_RenderCopy( renderer, textures[WRITEYOUROWN], NULL,  &destf);
    }

    // Here we change and stop animations, when required 
    // frame has been reached.
    if ( frame > 13 && type == SPEAK_RIGHT)
    {
      frame=0;
      destf.x = 120;
      type = SPEAK_SCREEN;
    } 
    else if ( frame > 13 && type == SPEAK_SCREEN )
    {
      frame=0;
      bAnimating = false;
    }
    
    // Some delay so animation is not too fast 
    // (just for the demo, in real world you have to 
    // compute correct frame by measuring passed time).
    
    SDL_Delay(200);

    SDL_RenderPresent( renderer );
      
  }

  // Cleanup
  SDL_DestroyTexture(pAnim );
  SDL_DestroyRenderer(renderer );
  SDL_DestroyWindow(window );
  TTF_CloseFont(pFont);
  TTF_Quit();
  SDL_Quit();


  return 0;
}
