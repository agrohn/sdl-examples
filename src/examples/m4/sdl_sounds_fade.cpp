////////////////////////////////////////////////////////////////////////////////
// SDL Programming
////////////////////////////////////////////////////////////////////////////////
// Demonstrates how to fade samples in and out using SDL mixer.
// 
// Instructions: press SPACEBAR to fade sound effect in and RETURN to fade sound
// out. Fade in and fade out take 3 seconds to complete.
//
/// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////

#include <SDL.h>  
#include <SDL_ttf.h>  
#include <SDL_mixer.h>
#include <iostream> 
#include <sstream>
#include <stdexcept>

const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;

using namespace std;
////////////////////////////////////////////////////////////////////////////////
// Handy function for creating a volume text.
void CreateStatusText( SDL_Renderer * renderer, TTF_Font *pFont, 
			 SDL_Texture* & pText, Mix_Fading state )
{
  stringstream s;
  s << "State : ";
  SDL_Color col = {255,255,255};
  if ( pText ) SDL_DestroyTexture( pText );

  switch ( state )
  {
  case MIX_NO_FADING:
    s << " Normal.";
    break;
  case MIX_FADING_IN:
    s << " Fading in.";
    break;
  case MIX_FADING_OUT:
    s << " Fading out.";
    break;
  }
  SDL_Surface *pSurface =  TTF_RenderText_Solid( pFont, s.str().c_str(), col );
  pText = SDL_CreateTextureFromSurface(renderer, pSurface);
  SDL_FreeSurface(pSurface);
}
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv )
{

  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
  {
    throw runtime_error(SDL_GetError());
  }
  // Initialize SDL_ttf library
  if ( TTF_Init() < 0 )
    throw runtime_error(TTF_GetError());
 

  
  SDL_Window *window = SDL_CreateWindow( "SDL_mixer fading",
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

  
  TTF_Font *pFont = TTF_OpenFont("res/efmi.ttf", 19);
  
  if ( pFont == NULL )
  {
    throw runtime_error(TTF_GetError());
  }

  
  /* load support for the OGG and MP3 sample/music formats*/
  int iDesiredFlags  = MIX_INIT_OGG | MIX_INIT_MP3;
  int iInitedFlags   = Mix_Init(iDesiredFlags);

  if( (iDesiredFlags & iInitedFlags) != iDesiredFlags) 
  {
    throw runtime_error(Mix_GetError());
  }
  /* Audio quality flags and values */
  int	 audio_freq     = 44100;
  Uint16 audio_format   = AUDIO_S16SYS;
  int	 audio_channels = 2;
  int	 audio_buffers  = 1024;

  if ( Mix_OpenAudio( audio_freq, audio_format, audio_channels, audio_buffers ) )
  {
    throw runtime_error(Mix_GetError());
  }


  // Load sound
  Mix_Chunk *pSound = NULL;
  pSound = Mix_LoadWAV("res/rain.wav");
  if ( pSound == NULL ) throw runtime_error(Mix_GetError());

  bool bRunning = true;

  
  SDL_Texture *pText = NULL;

  CreateStatusText( renderer, pFont, pText, MIX_NO_FADING );



  while ( bRunning )
  {

    SDL_Event ev;

    while ( SDL_PollEvent( & ev )) 
    {
      switch( ev.type )
      {
      case SDL_QUIT: 
	bRunning = false;
	break;
      case SDL_KEYDOWN:
	
	switch( ev.key.keysym.sym )
	{
	case SDLK_SPACE: 
	  // Fades channel in within 3 seconds.
	  Mix_FadeInChannel(0, pSound, -1, 3000 ); 
	  break;
	case SDLK_RETURN:
	  // Fades channel out within 3 seconds.
	  Mix_FadeOutChannel(0, 3000 ); 
	  break;
	default:
	  break;
	}
	break;
      default:
	break;
      }
    }  
    CreateStatusText( renderer, pFont, pText, Mix_FadingChannel(0) );
    SDL_RenderClear(renderer);
    SDL_Rect dst = { 10,100 };
    SDL_QueryTexture(pText, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy( renderer, pText, NULL,  &dst);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture( pText );
  Mix_FreeChunk(pSound);
  Mix_CloseAudio();
  Mix_Quit();
  SDL_Quit();
  
  return 0;
}

