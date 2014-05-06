////////////////////////////////////////////////////////////////////////////////
// SDL Programming 
////////////////////////////////////////////////////////////////////////////////
// Playing a sound in SDL using SDL-mixer.
/// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <SDL_mixer.h>
#include <iostream> 
#include <stdexcept>

const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;
const int COLOR_DEPTH_IN_BITS = 32;

using namespace std;

int main( int argc, char **argv )
{
  if ( SDL_Init( SDL_INIT_AUDIO ) < 0 )
  {
    throw runtime_error(SDL_GetError());
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

  Mix_Chunk *pSound = NULL;
  pSound = Mix_LoadWAV("res/burp-1.wav");
  if( pSound == NULL ) throw runtime_error(Mix_GetError());
  int chan = Mix_PlayChannel(-1, pSound, 0);
  while ( Mix_Playing(chan) != 0 );

  Mix_FreeChunk(pSound);
  Mix_CloseAudio();
  Mix_Quit();
  SDL_Quit();
  return 0;
}

