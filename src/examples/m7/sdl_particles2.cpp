#include <SDL.h>    
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>
#include <cmath>
#include <iostream> 
#include <vector>
#include <stdexcept>
#include <CParticleSystem.h>
#include <CVector2.h>
#include <CTimer.h>
using namespace std;
////////////////////////////////////////////////////////////////////////////////
const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;

////////////////////////////////////////////////////////////////////////////////
class CSimpleParticleSystem : public CParticleSystem
{
protected:
  CVector2f m_vPos;
  SDL_Texture * m_Image;
public:
  
  CSimpleParticleSystem( SDL_Renderer * renderer, 
			 size_t nNumParticles, 
			 const CVector2f & vPos) : 
    CParticleSystem(nNumParticles)
  {
    m_fEnergyDecrementPerSec = 0.55;
    m_vPos = vPos;
    m_Image = IMG_LoadTexture(renderer, "res/particle.png");
     if ( m_Image == NULL ) throw runtime_error(IMG_GetError());
  }
  void InitVelocity( CParticle & p) 
  {
    const int MAX = 201;
    p.m_vVelocity[0] = (rand() % MAX) - MAX/2;
    p.m_vVelocity[1] = (rand() % MAX) - MAX/2;
  }
  
  void InitPosition( CParticle & p) 
  {
    p.m_vPosition = m_vPos;
  }

  void InitEnergy ( CParticle & p ) 
  {
    p.m_fEnergy = 1.0;
  }
  void Render( SDL_Renderer * renderer )
  {
    for( size_t p=0; p< m_nNumParticlesAlive; p++)
    {
      CParticle & par = m_vecParticles[p];
      int w,h;
      SDL_QueryTexture( m_Image, NULL, NULL, &w, &h);
      
      SDL_Rect dest =  { static_cast<int>(par.m_vPosition[0]), 
			 static_cast<int>(par.m_vPosition[1]),
			 w, h };
      
      SDL_RenderCopy( renderer, m_Image, NULL, &dest);
    }
  }
};
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv )
{
  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    throw runtime_error(SDL_GetError());
  
  SDL_Window *window = SDL_CreateWindow( "particles example",
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


  const int MAX_NUMBER_OF_PARTICLES = 500;
  CVector2f pos(320,240);

  CSimpleParticleSystem system(renderer, MAX_NUMBER_OF_PARTICLES, pos);

  CTimer t;
  t.Reset();

  bool bRunning = true;
  
  while ( bRunning )
  {
    t.Update();
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
	case SDLK_SPACE:
	  system.Initialize(10);
	  break;
	default:
	  break;
	}
	break;
      }
    }
    SDL_SetRenderDrawColor(renderer,0,0,0,0);
    SDL_RenderClear(renderer);

    // Make sure that time has passed between updates
    if( t.GetPassedTimeMS() >= 5)
    {
      system.Update( t.GetPassedTime() );
      t.Reset();
    }
    system.Render( renderer );

    SDL_RenderPresent(renderer);

  }
  SDL_Quit();

  return 0;
}
////////////////////////////////////////////////////////////////////////////////
/// SDL Programming 2010
////////////////////////////////////////////////////////////////////////////////
/// Example of particle system, that allows to modify particle starting 
/// position via inheritance and function overriding.
/// Hit SPACEBAR to start effect. Hold SPACEBAR to see more.
/// \author anssi.grohn@pkamk.fi
////////////////////////////////////////////////////////////////////////////////
