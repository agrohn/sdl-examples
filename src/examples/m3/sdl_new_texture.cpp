////////////////////////////////////////////////////////////////////////////////
// SDL Programming
////////////////////////////////////////////////////////////////////////////////
// Example of rendering into texture. Utilizes same code as font example, but
// text is rendered to a texture, which is replicated to four parts of screen.
// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <iostream> 
#include <stdexcept>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>

using namespace std;

const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;

int main( int argc, char **argv )
{
  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
  {
    throw runtime_error(SDL_GetError());
  }
  
  // Initialize SDL_ttf library
  if ( TTF_Init() < 0 )
    throw runtime_error(TTF_GetError());
  
  SDL_Window *window = SDL_CreateWindow( "SDL render to texture example",
					 SDL_WINDOWPOS_CENTERED,
					 SDL_WINDOWPOS_CENTERED,
					 WINDOW_WIDTH, 
					 WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE
					 );
  
  if ( window == NULL )
    throw runtime_error(SDL_GetError());
  
  // Important: SDL_RENDERER_TARGETTEXTURE is required if you 
  // intend to draw stuff into a texture instead of window. 
  // You can still draw to a window with same renderer, even 
  // if SDL_RENDERER_TARGETTEXTURE is used.
  SDL_Renderer *renderer = SDL_CreateRenderer( window, -1, 
					       SDL_RENDERER_SOFTWARE |
					       SDL_RENDERER_TARGETTEXTURE );
  
  if ( renderer == NULL )
    throw runtime_error(SDL_GetError());



  // Create empty texture with screen dimensions, 
  // with alpha channel, 8-bits per channel.
  // SDL_TEXTUREACCESS_TARGET is required for rendering to a texture.
  SDL_Texture * renderToTexture = SDL_CreateTexture( renderer, 
						     SDL_PIXELFORMAT_RGBA8888,
						     SDL_TEXTUREACCESS_TARGET,
						     640, 480);
  if ( renderToTexture == NULL ) 
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
    // Set render target to texture.
    SDL_SetRenderTarget(renderer, renderToTexture);
    // Clear renderTexture contents to black first
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);

    // Fill half of the screen with orange box
    boxRGBA(renderer, 0,0, WINDOW_WIDTH*0.5, WINDOW_HEIGHT, 255,128,0,255);

    // Render text textures and get proper texture dimensions while doing it

    // Render solid text first...
    SDL_Rect dest = {120, 120, 0,0};
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



    // Change back to default render target ( window );
    SDL_SetRenderTarget(renderer, NULL);
    // Clear with black color 
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);
    
    
    // Render texture four times, scaled down. First one is at top left corner.
    SDL_Rect d = { 0, 0, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 };
    SDL_RenderCopy( renderer, renderToTexture, NULL, &d);
    // ... second one right below it...
    d.y += WINDOW_HEIGHT/2;
    SDL_RenderCopy( renderer, renderToTexture, NULL, &d);

    // ... third goes to bottom right corner ...
    d.x += WINDOW_WIDTH/2;
    SDL_RenderCopy( renderer, renderToTexture, NULL, &d);
    // ... and last one to top right corner.
    d.y = 0;
    SDL_RenderCopy( renderer, renderToTexture, NULL, &d);

    // Update screen
    SDL_RenderPresent( renderer );
    
  }
  
  // TTF cleanup 
  TTF_CloseFont(pFont);
  TTF_Quit();
  // delete textures
  SDL_DestroyTexture( renderToTexture);
  for(int i=0;i<NUM_SURFACES;i++) {
    SDL_DestroyTexture(pTextures[i]);
  }
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}
