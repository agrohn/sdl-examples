////////////////////////////////////////////////////////////////////////////////
// SDL Programming 
////////////////////////////////////////////////////////////////////////////////
// Application which demonstrates how channel playback can be stopped after 
// certain amount of milliseconds.
// 
// Instructions: press RETURN to start playback and SPACEBAR to make it stop 
// after half a second.
// 
/// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////

#include <SDL.h>    
#include <SDL_mixer.h>
#include <iostream> 
#include <sstream>
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
    SDL_Window *window = SDL_CreateWindow( "SDL_mixer timed halting",
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
  int channel = Mix_PlayChannel(-1, pSound, -1 ); 
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
	  Mix_ExpireChannel(channel, 500); // stop after half a second
	  break;
	case SDLK_RETURN:
	  if ( ! Mix_Playing( channel ) )
	    channel = Mix_PlayChannel(-1, pSound, -1 ); 
	  break;
	default:
	  break;
	}
	break;
      default:
	break;
      }
    }  

  }
  
  Mix_FreeChunk(pSound);
  Mix_CloseAudio();
  Mix_Quit();
  SDL_Quit();
  
  return 0;
}

