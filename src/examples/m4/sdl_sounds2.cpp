////////////////////////////////////////////////////////////////////////////////
// SDL Programming 
////////////////////////////////////////////////////////////////////////////////
// Playing a several sounds in SDL using SDL-mixer.
// Instructions: Press keys 1, 2 and 3 quickly to play several samples 
// simultaneously.
/// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <SDL_mixer.h>
#include <SDL_ttf.h>
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
  SDL_Surface *pText = TTF_RenderText_Solid(pFont,"Press 1, 2 and 3 in order to play sounds simultaneously.", col);  
  if ( pText == NULL ) throw runtime_error(SDL_GetError());
  SDL_Texture *pTextTex = SDL_CreateTextureFromSurface( renderer, pText);
  if ( pTextTex == NULL ) throw runtime_error(SDL_GetError());
  SDL_FreeSurface(pText);

  // Load sounds
  Mix_Chunk *pSoundBurp = NULL;
  Mix_Chunk *pSoundYes =  NULL;
  Mix_Chunk *pSoundSecret =  NULL;
  
  pSoundBurp = Mix_LoadWAV("./res/burp-1.wav");
  if ( pSoundBurp == NULL ) throw runtime_error(Mix_GetError());

  pSoundSecret = Mix_LoadWAV("./res/can-you-keep-a-secret.wav");
  if ( pSoundSecret == NULL ) throw runtime_error(Mix_GetError());

  pSoundYes = Mix_LoadWAV( "./res/yes-indeed.wav" );
  if ( pSoundYes == NULL ) throw runtime_error(Mix_GetError());

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
      case SDL_KEYDOWN:
	
	
	// each sound will be played on its own channel 
	// and mixed together
	switch( ev.key.keysym.sym )
	{
	case SDLK_1:
	  Mix_PlayChannel(-1, pSoundSecret, 0);
	  break;
	case SDLK_2:
	  Mix_PlayChannel(-1, pSoundYes, 0);
	  break;
	case SDLK_3:
	  Mix_PlayChannel(-1, pSoundBurp, 0);
	  break;
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
    SDL_QueryTexture(pTextTex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy( renderer, pTextTex, NULL, &dst);
    SDL_RenderPresent(renderer);
  }
  

  Mix_FreeChunk(pSoundBurp);
  Mix_FreeChunk(pSoundYes);
  Mix_FreeChunk(pSoundSecret);

  Mix_CloseAudio();
  Mix_Quit();
  SDL_DestroyTexture(pTextTex);
  SDL_Quit();
  
  return 0;
}

