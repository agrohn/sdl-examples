////////////////////////////////////////////////////////////////////////////////
// SDL Programming 
////////////////////////////////////////////////////////////////////////////////
// Playing timed sounds using SDL-mixer.
//
// Instructions: Press keys 1, 2 and 3 to play same sound effect for 500 ms,
// 700 ms, and 1 second.
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
    cerr << "Mix_Init: Failed to init required ogg and mod support!\n";
    cerr << "Mix_Init: " << Mix_GetError() << "\n";
    return 1;
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
  pSound = Mix_LoadWAV("res/can-you-keep-a-secret.wav");
  if ( pSound == NULL ) throw runtime_error(Mix_GetError());
  bool bRunning = true;

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
	case SDLK_1:
	  Mix_PlayChannelTimed(-1, pSound, -1, 500 );   // play 500ms
	  break;
	case SDLK_2:
	  Mix_PlayChannelTimed(-1, pSound, -1, 700 );  // play 700ms
	  break;
	case SDLK_3:
	  Mix_PlayChannelTimed(-1, pSound, -1, 1000 );  // play 1s
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

