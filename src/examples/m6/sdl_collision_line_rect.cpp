////////////////////////////////////////////////////////////////////////////////
/// SDL Programming
////////////////////////////////////////////////////////////////////////////////
/// Example of a collision detection using line and a circle. 
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
#include <algorithm>
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
class Rectangle
{
private:
  Vector2 m_vCenter;
  int m_iWidth;
  int m_iHeight;
public:

  Rectangle() {}

  Rectangle( int x, int y, int w, int h) 
  { 
    m_vCenter.SetX(x);
    m_vCenter.SetY(y);
    m_iWidth = w;
    m_iHeight = h;
  }
  
  int GetX() const { return static_cast<int>(m_vCenter.GetX()-(m_iWidth/2));}
  int GetY() const { return static_cast<int>(m_vCenter.GetY()-(m_iHeight/2));}

  int GetX2() const { return static_cast<int>(m_vCenter.GetX()+(m_iWidth/2));}
  int GetY2() const { return static_cast<int>(m_vCenter.GetY()+(m_iHeight/2));}

  Vector2 GetTopLeftCorner()
  {
    return Vector2(m_vCenter.GetX()-m_iWidth/2, m_vCenter.GetY()-m_iHeight/2);
  }
  
  int GetWidth() const { return m_iWidth; }
  int GetHeight() const { return m_iHeight; }
  
  void SetWidth( int w ) { m_iWidth = w; }
  void SetHeight( int h ) { m_iHeight = h; }

  void SetCenter( const Vector2 & vPos ) { m_vCenter = vPos; }
  const Vector2 & GetCenter() const { return m_vCenter; }

  void Move( const Vector2 & v ) { m_vCenter = m_vCenter + v;  }
};
////////////////////////////////////////////////////////////////////////////////
void
draw_rectangle( SDL_Renderer *renderer, Rectangle & rect, int r, int g, int b, int a  )
{
  boxRGBA(renderer, rect.GetX(), rect.GetY(), rect.GetX2(), rect.GetY2(), r,g,b,a );
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
bool collides( const Rectangle & r, const Vector2 & point )
{
  return ( point.GetX() >= r.GetX() && point.GetX() <= r.GetX2() &&
	   point.GetY() >= r.GetY() && point.GetY() <= r.GetY2() );
}
////////////////////////////////////////////////////////////////////////////////
bool 
collides( const Rectangle & r, const LineSegment & l)
{
  
  if ( collides(r, l.GetStart())) return true;
  if ( collides(r, l.GetEnd()))   return true;

  
  Vector2 posToCenter = r.GetCenter() - l.GetStart();
  Vector2 dirVec      = l.GetEnd()    - l.GetStart();

  float segmentLength = dirVec.Length();
  // direction vector must be unit vector (ie. length = 1) in this case!
  // otherwise we would need another formula for scalar projection.
  dirVec = dirVec / segmentLength; 

  // scalar projection of posToCenter to direction vector.
  float d = dirVec.Dot(posToCenter);

  // compute closest point to rect center from line start 
  // along direction vector.
  Vector2 closest_point =l.GetStart() + dirVec * d;
  
  Vector2 S = closest_point - r.GetCenter();
  Vector2 Sdir = S / S.Length();
  float eff_radius = ( fabsf(Vector2( r.GetWidth()*0.5f,0).Dot(Sdir)), 
		       fabsf(Vector2( 0, r.GetHeight()*0.5f).Dot(Sdir))) ;
  return ( eff_radius >= S.Length());

}
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv )
{
  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    throw runtime_error(SDL_GetError());

  SDL_Window *window = SDL_CreateWindow( "Line and rectangle collision example",
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
  line.SetStart(0,0);
  line.SetEnd(20,40); 

  Rectangle rect(250,150,150,50);

  
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

      if ( collides( rect, line) )
      {
	draw_rectangle( renderer, rect, 255,0,0,255);
      }
      else
	draw_rectangle( renderer, rect, 255,127,0,255);

      SDL_RenderPresent( renderer );
      SDL_Delay(1);
    }
  }
  SDL_Quit();
  return 0;
}
