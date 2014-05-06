////////////////////////////////////////////////////////////////////////////////
// SDL Programming 
////////////////////////////////////////////////////////////////////////////////
// Playing, pausing, resuming and controlling sample volume in SDL using SDL-mixer.
//
// Instructions: Press arrow keys UP and DOWN to change sound effect volume,
//		 SPACEBAR to pause and resume playback, and Q to halt playback.
/// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream> 
#include <sstream>
#include <stdexcept>

const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;


using namespace std;
// Handy function for creating a volume text.
void CreateVolumeText( SDL_Renderer * renderer, TTF_Font *pFont, SDL_Texture* & pTexture, int volume )
{
  stringstream s;
  s << "Volume : " << volume;
  SDL_Color col = {255,255,255};
  if ( pTexture ) SDL_DestroyTexture( pTexture );
  SDL_Surface * tmp = TTF_RenderText_Solid( pFont, s.str().c_str(), col );
  pTexture =  SDL_CreateTextureFromSurface(renderer, tmp);
  SDL_FreeSurface(tmp);
}

int main( int argc, char **argv )
{
  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
  {
    throw runtime_error(SDL_GetError());
  }
  // Initialize SDL_ttf library
  if ( TTF_Init() < 0 )
    throw runtime_error(TTF_GetError());
 

  
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



  TTF_Font *pFont = TTF_OpenFont("./res/efmi.ttf", 19);
  
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
  SDL_Color col = {255,255,255};

  SDL_Surface *tmp = NULL;
  tmp = TTF_RenderText_Solid(pFont,"Press arrow up and down to control volume, space pauses/unpauses.", col);  
  
  SDL_Texture *pText = SDL_CreateTextureFromSurface(renderer, tmp);
  SDL_FreeSurface(tmp);
  tmp = TTF_RenderText_Solid(pFont,"Press Q to stop playing on channel.", col);  

  SDL_Texture *pText2 = SDL_CreateTextureFromSurface(renderer, tmp);
  SDL_FreeSurface(tmp);

  SDL_Texture *pVolumeText = NULL;

  // Load sounds
  Mix_Chunk *pSoundRain = NULL;
  pSoundRain = Mix_LoadWAV("res/rain.wav");
  if ( pSoundRain == NULL ) throw runtime_error(Mix_GetError());

  int channel = Mix_PlayChannel(-1, pSoundRain, -1);


  bool bRunning = true;
  const int VOL_STEP = 3;

  // create volume indicator text
  CreateVolumeText( renderer, pFont, pVolumeText, Mix_VolumeChunk(pSoundRain, -1));
  
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
	case SDLK_UP:
	  {
	    int curr_vol = Mix_VolumeChunk(pSoundRain, -1);
	    curr_vol +=  VOL_STEP;
	    Mix_VolumeChunk( pSoundRain, curr_vol );

	    CreateVolumeText( renderer, pFont, pVolumeText, Mix_VolumeChunk(pSoundRain, -1));
	  }
	  break;
	case SDLK_DOWN:
	  {
	    int curr_vol = Mix_VolumeChunk(pSoundRain, -1);
	    curr_vol -= VOL_STEP;
	    if ( curr_vol < 0 ) curr_vol = 0;

	    Mix_VolumeChunk( pSoundRain, curr_vol );

	    CreateVolumeText( renderer, pFont, pVolumeText, Mix_VolumeChunk(pSoundRain, -1));
	  }
	  break;
	case SDLK_SPACE:

	  if ( Mix_Paused( channel ))   Mix_Resume( channel );
	  else				Mix_Pause( channel );

	  break;
	case SDLK_q:
	  Mix_HaltChannel( channel );
	default:
	  break;
	}
	break;
      default:
	break;
      }
    }  
    SDL_Rect dst = { 10,100 };
    SDL_RenderClear(renderer);

    SDL_QueryTexture(pText, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy( renderer, pText, NULL, &dst);
    dst.y += 20;
    SDL_QueryTexture(pText2, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy( renderer, pText2, NULL, &dst);
    dst.y += 20;
    SDL_QueryTexture(pVolumeText, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy( renderer, pVolumeText, NULL,  &dst);
    SDL_RenderPresent(renderer);
  }
  
  Mix_FreeChunk(pSoundRain);
  SDL_DestroyTexture( pText );
  SDL_DestroyTexture( pText2 );
  SDL_DestroyTexture( pVolumeText );
  Mix_CloseAudio();
  Mix_Quit();
  TTF_Quit();
  SDL_Quit();
  
  return 0;
}

