////////////////////////////////////////////////////////////////////////////////
/// SDL Programming
////////////////////////////////////////////////////////////////////////////////
/// Example of SDL_gfx graphics primitive operations.
/// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <SDL2_gfxPrimitives.h>
#include <iostream> 
#include <stdexcept>
const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;

using namespace std;


int main( int argc, char **argv )
{


  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    throw runtime_error( SDL_GetError());

  
  
  SDL_Window *pScreen = SDL_CreateWindow( "SDL Input handling example",
					  SDL_WINDOWPOS_CENTERED,
					   SDL_WINDOWPOS_CENTERED,
					   WINDOW_WIDTH, 
					   WINDOW_HEIGHT, 
					  SDL_WINDOW_SHOWN
					  );
  

  if ( pScreen == NULL ) throw runtime_error( SDL_GetError());


  SDL_Renderer *renderer = SDL_CreateRenderer( pScreen, -1, SDL_RENDERER_SOFTWARE);
  if ( renderer == NULL ) throw runtime_error( SDL_GetError());




  bool bRunning = true;



  int x1, y1;
  int x2, y2;
  int x3, y3;
  int width, height;
  int radius;
  bool fill = true;

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
	if ( ev.key.keysym.sym == SDLK_F1 && !ev.key.repeat ) 
	  fill = !fill;
	break;
      }
    }
    // clear screen. Draw color needs to be set since SDL_gfx will manipulate 
    // it.
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear(renderer);
    
    // small red square of four pixels
    pixelRGBA(renderer, 20,20, 255,0,0,255);
    pixelRGBA(renderer, 21,20, 255,0,0,255);
    pixelRGBA(renderer, 21,21, 255,0,0,255);
    pixelRGBA(renderer, 20,21, 255,0,0,255);

    x1 = 30;
    y1 = 20;
    x2 = 30;
    y2 = 50;
    // draws yellow line 
    lineRGBA(renderer, x1,y1, x2, y2, 255,255,0,255 );
    
    x1 = 40;
    y1 = 20;
    x2 = 70;
    y2 = 40;

    // draws magenta line 
    lineRGBA(renderer, x1,y1, x2, y2, 255,0,255,255 );

    x1 = 80;
    y1 = 20;
    x2 = 110;
    y2 = 50;
    
    // draws yellow rectangle outline
    rectangleRGBA(renderer, x1, y1, x2, y2, 255, 255, 0, 255);    
    
    x1 = 120;
    y1 = 20;
    x2 = 150;
    y2 = 50;
    
    // draws orange filled rectangle 
    boxRGBA(renderer, x1,y1,x2,y2, 255, 125, 0, 255);    
    
    x1 = 200, y1 = 20;
    x2 = 200, y2 = 50;
    x3 = 250, y3 = 50;
    
    //  draws greenish filled triangle
    filledTrigonRGBA( renderer, 
		      x1,y1,
		      x2,y2,
		      x3,y3,
		      125,255,0,255);
    

    x1 = 300, y1 = 20;
    x2 = 350, y2 = 20;
    x3 = 350, y3 = 50;
    //  draws light blue triangle outline
    trigonRGBA( renderer, 
		x1,y1,
		x2,y2,
		x3,y3,
		155,155,255,255);
    
    x1 = 30;
    y1 = 150;
    radius = 25;
    // draws circle turqoise circle outline
    circleRGBA( renderer, x1, y1, radius, 0,255,135, 255);

    x1 = 100;
    y1 = 150;
    radius = 25;
    // draws yellow filled circle 
    filledCircleRGBA( renderer, x1, y1, radius, 255,255,0,255);
    
    x1 = 200;
    y1 = 150;
    width = 60;
    height = 21;
    // draws blueish ellipse outline
    ellipseRGBA( renderer, x1,y1,width,height, 125,125,255,255);

    x1 = 350;
    y1 = 150;
    width = 25;
    height = 44;
    // draws blueish filled ellipse outline
    filledEllipseRGBA(renderer, x1,y1,width,height,0,22,255,255);

    const int NUM_POINTS = 5;
    short pointsX[NUM_POINTS] = { 150, 200, 175, 125, 100 };
    short pointsX2[NUM_POINTS] = { 250, 300, 275, 225, 200 };
    short pointsY[NUM_POINTS] = { 250, 300, 350, 350, 300 };

    // Draws polygon outline
    polygonRGBA(renderer, 
		pointsX, pointsY,
		NUM_POINTS, 255,255,255,255);


    // Draws filled polygon 
    filledPolygonRGBA(renderer, 
		      pointsX2, pointsY,
		      NUM_POINTS, 255,0,0,255);
    
    // draws three partially transparent green rectangles over previous drawings
    // alpha value 55  = 78 % transparent, 22% opaque
    // alpha value 128 = 50 % transparent, 50% opaque
    // alpha value 205 = 20 % transparent, 80% opaque
    if ( fill )
    {
      boxRGBA(renderer, 10,10,100,400, 0, 255, 0,  55);
      boxRGBA(renderer, 100,10,200,400, 0,255,0,  128);
      boxRGBA(renderer, 200,10,300,400, 0, 255,0, 205);
    }
    SDL_Delay(5);

    SDL_RenderPresent( renderer );
  }
  SDL_Quit();
  return 0;

}
