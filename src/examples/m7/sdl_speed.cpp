////////////////////////////////////////////////////////////////////////////////
/// SDL Programming
////////////////////////////////////////////////////////////////////////////////
/// Example of time-based movement. Usage:
/// Press left arrow key to make plane move towards left edge, right arrow key to
/// make plane move towards right edge. 
/// Press arrow keys multiple times increase speed. You can reset simulation back to 
/// original state by pressing R.
/// 
/// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <cmath>
#include <iostream> 
#include <stdexcept>

using namespace std;

const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;
const int COLORKEY[3] = {0, 67, 171};

/////////////////////////////////////////////////////////////////
// Handy 2D vector class.
class Vector2
{
private:
  float m_values[2];

public:
  Vector2() { } // So that object can be created without values
  
  Vector2( const float & x, const float & y ) 
  {
    m_values[0] = x;
    m_values[1] = y;
  }
  
  const float & GetX() const { return m_values[0]; }
  const float & GetY() const { return m_values[1]; }

  void SetX( const float & x ) { m_values[0] = x; }
  void SetY( const float & y ) { m_values[1] = y; }

  float Length() const
  {
    return sqrt(m_values[0]*m_values[0] + m_values[1] * m_values[1]);
  }
  
  Vector2 operator+( const Vector2 & v ) const
  {
    return Vector2( m_values[0] + v.m_values[0], m_values[1] + v.m_values[1] );
  }
  Vector2 operator-( const Vector2 & v ) const
  {
    return Vector2( m_values[0] - v.m_values[0], m_values[1] - v.m_values[1] );
  }
  float & operator[](size_t index) 
  {
    return m_values[index];
  }
  const float & operator[](size_t index)  const
  {
    return m_values[index];
  }
  float Dot( const Vector2 & v ) const
  {
    return (m_values[0]*v.m_values[0] + m_values[1] *v.m_values[1]);
  }
  
};
////////////////////////////////////////////////////////////////////////////////
class Plane
{
private:
  Vector2	m_vPosition;  // current position of game object.
  Vector2	m_vVelocity;  // velocity vector, length of velocity = speed.
  SDL_Texture * m_pImage;     // game object graphical representation
public:
  ////////////////////
  //
  Plane( SDL_Renderer * renderer )
  {
    // Attempt to load image
    SDL_Surface *tmp = SDL_LoadBMP("res/plane.bmp");
    
    if ( tmp  == NULL  ) throw runtime_error("No image loaded");


    Uint32 colorkey = SDL_MapRGB( tmp->format, 
				  COLORKEY[0], 
				  COLORKEY[1],
				  COLORKEY[2] );

    // Set colorkey for next image
    SDL_SetColorKey( tmp, SDL_TRUE, colorkey);
    m_pImage = SDL_CreateTextureFromSurface( renderer, tmp);
    if ( m_pImage == NULL ) throw runtime_error(SDL_GetError());
    
    m_vVelocity[0] = 0.0f;
    m_vVelocity[1] = 0.0f;
  }
  ////////////////////
  //
  ~Plane()
  {
    // Release image 
    SDL_DestroyTexture(m_pImage);

  }
  ////////////////////
  // Update object position.
  void Update( float fSeconds )
  {
    m_vPosition[0] += m_vVelocity[0] * fSeconds ; 
    m_vPosition[1] += m_vVelocity[1] * fSeconds ; 
  }
  ////////////////////
  // Halt movement ( speed = 0.0f ).
  void Stop()
  {
    m_vVelocity[0] = 0.0f;
    m_vVelocity[1] = 0.0f;
  }
  // Sets velocity
  void SetVelocity( const Vector2 & v )
  {
    m_vVelocity = v;
  }
  ////////////////////
  // Gets velocity
  Vector2 & GetVelocity()
  {
    return m_vVelocity;
  }
  ////////////////////
  /// Draws object on screen.
  void Render( SDL_Renderer *renderer )
  {
    int w,h;
    SDL_QueryTexture(m_pImage, NULL, NULL, &w,&h);
    SDL_Rect dst = { (int)m_vPosition[0]-w/2, (int)m_vPosition[1]-h/2, w,h };
    SDL_RenderCopy( renderer, m_pImage, NULL, &dst);
  }
  ////////////////////
  // Sets position
  void SetPosition( const Vector2 & v)
  {
    m_vPosition = v;
  }
};
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv )
{
  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    throw runtime_error(SDL_GetError());

  SDL_Window *window = SDL_CreateWindow( "Speed example",
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
  
  Plane plane(renderer);
  plane.SetPosition( Vector2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2));
  bool bRunning = true;
  Vector2 velocityChange(65,0);
  
  Uint32 ticks_prev = SDL_GetTicks();


  while ( bRunning )
  {
    Uint32 ticks_curr = SDL_GetTicks();
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
	case SDLK_LEFT:
	  // increase speed to left
	  plane.SetVelocity( plane.GetVelocity() - velocityChange );
	  break;
	case SDLK_RIGHT:
	  // increase speed to right
	  plane.SetVelocity( plane.GetVelocity() + velocityChange );
	  break;
	case SDLK_r:
	  plane.SetPosition( Vector2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2));
	  plane.Stop();
	  break;
	default:
	  break;
	}
	break;
      }
    }
    
    if ( ticks_curr > 5 )
    {
      plane.Update( (ticks_curr - ticks_prev)*0.001f );      
      ticks_prev = ticks_curr;
    }
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear( renderer);

    plane.Render( renderer );
    
    
    SDL_RenderPresent( renderer );
  }
  SDL_Quit();
  return 0;
}
