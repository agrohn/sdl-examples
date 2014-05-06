////////////////////////////////////////////////////////////////////////////////
// SDL Programming
////////////////////////////////////////////////////////////////////////////////
// Playing music in SDL using SDL-mixer.
/// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <SDL_mixer.h>
#include <iostream> 
#include <stdexcept>

const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;

using namespace std;

int main( int argc, char **argv )
{
  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
  {
    throw runtime_error(SDL_GetError());
  }
  
  SDL_Window *window = SDL_CreateWindow( "SDL_mixer example 2",
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

  /* load support for the OGG and MP3 sample/music formats*/
  int iDesiredFlags  = MIX_INIT_OGG | MIX_INIT_MP3;
  int iInitedFlags   = Mix_Init(iDesiredFlags);

  if( (iDesiredFlags & iInitedFlags) != iDesiredFlags) 
  {
    throw runtime_error(Mix_GetError());
  }

  int audio_rate = 44100;
  Uint16 audio_format = AUDIO_S16SYS;
  int audio_channels = 2;
  int audio_buffers  = 4096;

  if ( Mix_OpenAudio( audio_rate, audio_format, audio_channels, audio_buffers ) )
  {
    throw runtime_error(Mix_GetError());
  }
  
  // Music piece Super Metroid 'The Galaxy Awaits' by Nutritious, 
  // available in OverClocked ReMix (http://www.ocremix.org)"
  // http://www.ocremix.org/remix/OCR01804/

  Mix_Music *pMusic = NULL;
  pMusic = Mix_LoadMUS("res/music.ogg");

  if( pMusic == NULL) 
  {
    throw runtime_error(Mix_GetError());
  }

  if ( Mix_PlayMusic( pMusic, -1) == -1 ) 
  {
    throw runtime_error(Mix_GetError());
  }

  bool bRunning = true;
  while( bRunning && Mix_PlayingMusic() )
  {
    SDL_Event ev;
    while ( SDL_PollEvent( & ev ))  // Check event queue.
    {
      switch( ev.type )
      {
      case SDL_QUIT: // when window is closed
	bRunning = false;
	break;
      default:
	break;
      }
    }
  }
  
  Mix_FreeMusic(pMusic);
  Mix_CloseAudio();
  Mix_Quit();
  SDL_Quit();
  return 0;
}

