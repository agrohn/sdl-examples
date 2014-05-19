////////////////////////////////////////////////////////////////////////////////
/// SDL Programming
////////////////////////////////////////////////////////////////////////////////
/// Example of a collision detection using line segment and a circle. 
/// Drag mouse with left mouse button pressed to see collision effect.
/// When line segment crosses circle, it is drawn red.
/// 
/// If you wish to move the other end, click with right mouse button.
///
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
  
  float Dot( const Vector2 & v ) const
  {
    return (m_values[0]*v.m_values[0] + m_values[1] *v.m_values[1]);
  }

  Vector2 operator/( float value ) const
  {
    return Vector2( m_values[0] / value, m_values[1] / value );
  }
  Vector2  operator*(float value ) const
  {
    return Vector2( m_values[0] * value, m_values[1] * value );
  }
};
////////////////////////////////////////////////////////////////////////////////
class LineSegment
{
private:
  Vector2 start;
  Vector2 end;
public:

  LineSegment() { }

  LineSegment( int x1,int y1, int x2, int y2 ) 
  { 
    start.SetX(x1); 
    start.SetY(y1); 
    end.SetX(x2);
    end.SetY(y2);
  }

  void SetStart( int x, int y) { start.SetX(x); start.SetY(y); }
  void SetEnd( int x, int y) { end.SetX(x); end.SetY(y); }
  
  const Vector2 & GetStart() const { return start; }
  const Vector2 & GetEnd() const { return end; }
};
////////////////////////////////////////////////////////////////////////////////
class Circle
{
private:
  Vector2 m_vCenter;
  int	       m_iRadius;
public:

  const Vector2 & GetCenter() const{ return m_vCenter; }
  void SetCenter( const Vector2 & center ) { m_vCenter = center; }
  
  void SetRadius( int val ) { m_iRadius = val; }
  int GetRadius() const { return m_iRadius; }
  void Move( Vector2 v ) { m_vCenter = m_vCenter + v;  }
};
////////////////////////////////////////////////////////////////////////////////
void
draw_circle( SDL_Renderer *renderer, Circle & c, int r, int g, int b, int a  )
{
  // just to make compiler happy
  int x = static_cast<int>(c.GetCenter().GetX());
  int y = static_cast<int>(c.GetCenter().GetY());
  filledCircleRGBA(renderer, x,y, c.GetRadius(), r,g,b,a );
}
////////////////////////////////////////////////////////////////////////////////
void
draw_line( SDL_Renderer *renderer, LineSegment & l, int r, int g, int b, int a )
{
  // just to make compiler happy
  int x1 = static_cast<int>(l.GetStart().GetX());
  int y1 = static_cast<int>(l.GetStart().GetY());
  int x2 = static_cast<int>(l.GetEnd().GetX());
  int y2 = static_cast<int>(l.GetEnd().GetY());
  
  lineRGBA( renderer, x1,y1,x2,y2, r,g,b,a);
}
////////////////////////////////////////////////////////////////////////////////
bool 
collides( const Circle & c, const LineSegment & l)
{
  
  Vector2 posToCenter = c.GetCenter() - l.GetStart();
  Vector2 dirVec      = l.GetEnd()    - l.GetStart();

  float segmentLength = dirVec.Length();
  // direction vector must be unit vector (ie. length = 1) in this case!
  // otherwise we would need another formula for scalar projection.
  dirVec = dirVec / segmentLength; 

  // scalar projection of posToCenter to direction vector.
  float d = dirVec.Dot(posToCenter);

  // if d value exceeds original segment length, then we put a cap on it.
  // if these two lines are dismissed, then algorithm sees line segment 
  // as a infinite line.
  if ( d >  segmentLength ) d = segmentLength;
  if ( d < -segmentLength ) d = -segmentLength;
  
  // compute closest point to circle center from line start 
  // along direction vector.
  Vector2 closest_point =l.GetStart() + dirVec * d;

  // vectorfrom circle center to closest point on line
  Vector2 S = closest_point - c.GetCenter();

  return (S.Length() <= c.GetRadius());
}
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv )
{


  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    throw runtime_error(SDL_GetError());

  SDL_Window *window = SDL_CreateWindow( "Circle and line collision example",
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



  bool bRunning = true;

  LineSegment line;
  line.SetStart(250,50);
  line.SetEnd(200,200); 

  Circle circle;
  circle.SetCenter( Vector2(250,150));
  circle.SetRadius(30);
  
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
      case SDL_MOUSEBUTTONDOWN:
	switch( ev.button.button )
	{
	case SDL_BUTTON_LEFT:
	  line.SetEnd( ev.button.x, ev.button.y );
	  break;
	case SDL_BUTTON_RIGHT:
	  line.SetStart( ev.button.x, ev.button.y );
	  break;
	default:
	  break;
	}
	break;
      case SDL_MOUSEMOTION:
	if ( ev.motion.state & SDL_BUTTON( SDL_BUTTON_LEFT ))
	{
	  line.SetEnd( ev.motion.x, ev.motion.y);
	}

	break;
      }

      SDL_SetRenderDrawColor(renderer,0,0,0,0);
      SDL_RenderClear(renderer);
      
      draw_line( renderer, line, 0,255,0,255);

      if ( collides( circle, line) )
      {
	draw_circle( renderer, circle, 255,0,0,255);
      }
      else
	draw_circle( renderer, circle, 255,127,0,255);

      SDL_RenderPresent( renderer );
      SDL_Delay(1);
    }
  }
  SDL_Quit();
  return 0;
}
