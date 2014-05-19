////////////////////////////////////////////////////////////////////////////////
/// SDL Programming 
////////////////////////////////////////////////////////////////////////////////
/// Example of a rectangle collision detection. Use arrow keys to move 
/// rectangles over each other. Static rectangle changes color when collision 
/// is detected.
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
class Rectangle
{
private:
  Vector2<int> m_vCenter;
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
  
  int GetX() const { return m_vCenter.GetX()-(m_iWidth/2);}
  int GetY() const { return m_vCenter.GetY()-(m_iHeight/2);}

  int GetX2() const { return m_vCenter.GetX()+(m_iWidth/2);}
  int GetY2() const { return m_vCenter.GetY()+(m_iHeight/2);}

  Vector2<int> GetTopLeftCorner()
  {
    return Vector2<int>(m_vCenter.GetX()-m_iWidth/2, m_vCenter.GetY()-m_iHeight/2);
  }
  
  int GetWidth() const { return m_iWidth; }
  int GetHeight() const { return m_iHeight; }
  
  void SetWidth( int w ) { m_iWidth = w; }
  void SetHeight( int h ) { m_iHeight = h; }

  void SetCenter( const Vector2<int> & vPos ) { m_vCenter = vPos; }
  const Vector2<int> & GetCenter() const { return m_vCenter; }

  void Move( Vector2<int> v ) { m_vCenter = m_vCenter + v;  }
};
////////////////////////////////////////////////////////////////////////////////
void
draw_rectangle( SDL_Renderer *renderer, Rectangle & rect, int r, int g, int b, int a  )
{
  boxRGBA(renderer, rect.GetX(), rect.GetY(), rect.GetX2(), rect.GetY2(), r,g,b,a );
}
////////////////////////////////////////////////////////////////////////////////
bool 
rectangles_collide( const Rectangle & r1, const Rectangle & r2)
{
  int r1_top = r1.GetY();
  int r2_top = r2.GetY();

  int r1_bottom = r1.GetY2();
  int r2_bottom = r2.GetY2();

  int r1_left = r1.GetX();
  int r2_left = r2.GetX();

  int r1_right = r1.GetX2();
  int r2_right = r2.GetX2();

  if ( r1_bottom < r2_top ) return false;
  if ( r1_top > r2_bottom ) return false;
  if ( r1_left > r2_right ) return false;
  if ( r1_right < r2_left ) return false;

  return true;
}
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv )
{
  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    throw runtime_error(SDL_GetError());

  SDL_Window *window = SDL_CreateWindow( "Rectangle collision example",
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


  

  const int STEP = 3;

  
  Rectangle r1, r2;
  r1.SetCenter( Vector2<int>(50,50));
  r1.SetHeight( 30);
  r1.SetWidth( 50 );

  r2.SetCenter( Vector2<int>(150,50));
  r2.SetHeight( 20);
  r2.SetWidth( 120);
  
  

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

	  r1.Move(Vector2<int>(0,STEP));
	  break;
	case SDLK_UP:

	  r1.Move(Vector2<int>(0,-STEP));
	  break;
	case SDLK_RIGHT:

	  r1.Move(Vector2<int>(STEP,0));
	  break;
	case SDLK_LEFT:

	  r1.Move(Vector2<int>(-STEP,0));
	  break;
	default:
	  break;
	}

	break;
      }
      SDL_SetRenderDrawColor(renderer,0,0,0,0);
      SDL_RenderClear(renderer);
      
      draw_rectangle( renderer, r1, 255,255,0,255);

      if ( rectangles_collide( r1, r2))
      {
	draw_rectangle( renderer, r2, 255,0,0,255);
      }
      else
	draw_rectangle( renderer, r2, 255,127,0,255);
      
      SDL_RenderPresent( renderer );
      SDL_Delay(1);
    }
  }
  SDL_Quit();
  return 0;
}
