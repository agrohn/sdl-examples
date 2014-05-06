
////////////////////////////////////////////////////////////////////////////////
// SDL Programming 
////////////////////////////////////////////////////////////////////////////////
// Example of very rudimentary positional audio sound effect in 
// SDL using SDL_mixer.
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
  
  SDL_Window *window = SDL_CreateWindow( "SDL_mixer sfx example position",
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
  

  
  Mix_Chunk *pSound = NULL;
  pSound = Mix_LoadWAV("res/rain.wav");
  
  if( pSound == NULL) 
  {
    throw runtime_error(Mix_GetError());
  }

  int channel = Mix_PlayChannel(-1, pSound, -1 );
  bool bRunning = true;
  int angle = 0;
  while( bRunning  )
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
    Mix_SetPosition( channel, angle, 127);
    angle++;
    angle = angle % 360;
    SDL_Delay(100);
  }
  
  Mix_FreeChunk(pSound);
  Mix_CloseAudio();
  Mix_Quit();
  SDL_Quit();
  return 0;
}
