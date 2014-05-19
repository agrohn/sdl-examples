////////////////////////////////////////////////////////////////////////////////
/// SDL Programming 
////////////////////////////////////////////////////////////////////////////////
/// Example of a circle-based collision detection. Use arrow keys to move 
/// smaller circle over the bigger one.
/// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <iostream> 
#include <cassert>
#include <stdexcept>
////////////////////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////////////////////
const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;
/////////////////////////////////////////////////////////////////
// Handy 2D vector class.
template<class T>
class Vector2
{
private:
  T m_values[2];

public:
  Vector2() { } // So that object can be created without values
  
  Vector2( const T & x, const T & y ) 
  {
    m_values[0] = x;
    m_values[1] = y;
  }
  
  const T & GetX() const { return m_values[0]; }
  const T & GetY() const { return m_values[1]; }

  void SetX( const T & x ) { m_values[0] = x; }
  void SetY( const T & y ) { m_values[1] = y; }

  T Length() const
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
  
  T Dot( const Vector2 & v ) const
  {
    return (m_values[0]*v.m_values[0] + m_values[1] *v.m_values[1]);
  }
  
};
////////////////////////////////////////////////////////////////////////////////
class Circle
{
private:
  Vector2<int> m_vCenter;
  int	       m_iRadius;
public:

  const Vector2<int> & GetCenter() const{ return m_vCenter; }
  void SetCenter( const Vector2<int> & center ) { m_vCenter = center; }
  
  void SetRadius( int val ) { m_iRadius = val; }
  int GetRadius() const { return m_iRadius; }
  void Move( Vector2<int> v ) { m_vCenter = m_vCenter + v;  }
};
////////////////////////////////////////////////////////////////////////////////
void
draw_circle( SDL_Renderer *renderer, Circle & c, int r, int g, int b, int a  )
{
  filledCircleRGBA(renderer, c.GetCenter().GetX(), c.GetCenter().GetY(), c.GetRadius(), r,g,b,a );
}
////////////////////////////////////////////////////////////////////////////////
bool 
circles_collide( const Circle & c1, const Circle & c2)
{
  Vector2<int> diff = c1.GetCenter() - c2.GetCenter();
  return  ( diff.Length() < (c1.GetRadius() + c2.GetRadius()));
}
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv )
{

  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    throw runtime_error(SDL_GetError());

  SDL_Window *window = SDL_CreateWindow( "Circle collision example",
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


  const int STEP = 3;

  
  Circle circle1, circle2;

  circle1.SetCenter( Vector2<int>(50,50));
  circle1.SetRadius(10);
  circle2.SetCenter( Vector2<int>(150,50));
  circle2.SetRadius(30);
  int bRunning = true;
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
	case SDLK_DOWN:

	  circle1.Move(Vector2<int>(0,STEP));
	  break;
	case SDLK_UP:

	  circle1.Move(Vector2<int>(0,-STEP));
	  break;
	case SDLK_RIGHT:

	  circle1.Move(Vector2<int>(STEP,0));
	  break;
	case SDLK_LEFT:

	  circle1.Move(Vector2<int>(-STEP,0));
	  break;
	default:
	  break;
	}

	break;
      }
      
      SDL_SetRenderDrawColor(renderer,0,0,0,0);
      SDL_RenderClear(renderer);
      
      draw_circle( renderer, circle1, 255,255,0,255);
      if ( circles_collide( circle1, circle2))
      {
	draw_circle( renderer, circle2, 255,0,0,255);
      }
      else
	draw_circle( renderer, circle2, 255,127,0,255);
      SDL_RenderPresent( renderer );
      SDL_Delay(2);
    }
  }
  SDL_Quit();
  return 0;
}
