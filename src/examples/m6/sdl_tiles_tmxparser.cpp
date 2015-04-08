////////////////////////////////////////////////////////////////////////////////
/// SDL Programming 
////////////////////////////////////////////////////////////////////////////////
/// Example of a tile-based graphics loaded using TMXParser library.
/// \author anssi.grohn@karelia.fi (c) 2010-2015
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <SDL_image.h>
#include <iostream> 
#include <stdexcept>
// headers for TmxParser
#include <Tmx.h> 
#include <sstream>
////////////////////////////////////////////////////////////////////////////////
const int WINDOW_WIDTH  = 640;
const int WINDOW_HEIGHT = 480;
const int COLOR_DEPTH_IN_BITS = 32;
////////////////////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////////////////////
void SetRect( SDL_Rect & r, int x, int y, int w, int h )
{
  r.x = x;
  r.y = y;
  r.w = w;
  r.h = h;
}
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv )
{

  if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
    throw runtime_error(SDL_GetError());

  SDL_Window *window = SDL_CreateWindow( "Tilemap example with TmxParser",
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

  // Load Tmx map
  Tmx::Map *map = new Tmx::Map();
  map->ParseFile("res/map.tmx");
  if ( map->HasError())
  {
    throw runtime_error(map->GetErrorText());
  }

  // make proper path from image name.
  stringstream ss;
  ss << "res/";
  ss << map->GetTileset(0)->GetImage()->GetSource();
  

  // Load tile data, this time as a surface.
  SDL_Surface *pTileSurface = IMG_Load( ss.str().c_str()  );
  if ( pTileSurface == NULL )
  {
    throw std::runtime_error(IMG_GetError());
  }


  // color is returned as hex triplet
  string color = map->GetTileset(0)->GetImage()->GetTransparentColor();
  int r,g,b;
  // reset stringstream and extract r-component from hex value
  ss.clear();
  ss.str(color.substr(0,2));
  ss >> std::hex >> r;
  // reset stringstream and extract g-component from hex value
  ss.clear();
  ss.str(color.substr(2,2));
  ss >> std::hex >> g;
  // reset stringstream and extract b-component from hex value
  ss.clear();
  ss.str(color.substr(4,2));
  ss >> std::hex >> b;

  // Set color key information for surface.
  SDL_SetColorKey( pTileSurface,
		   SDL_TRUE,
		   SDL_MapRGB( pTileSurface->format, r, g, b ));

  // Convert color keyed surface as texture
  SDL_Texture *pBackgroundTiles = SDL_CreateTextureFromSurface(renderer, pTileSurface);
  if ( pBackgroundTiles == NULL )
  {
    throw std::runtime_error(SDL_GetError());
  }
  
  // Remove unneeded surface
  SDL_FreeSurface( pTileSurface);

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
      }
    }
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);


    
    // Assuming layers are stored in correct order (lowest to topmost)
    for( const Tmx::TileLayer *layer : map->GetTileLayers())
    {
      //cout << "Drawing layer: " << layer->GetName() << "\n";

      for( int r=0;r<layer->GetHeight();r++)
      {
	for( int c=0;c<layer->GetWidth();c++)
	{
	  int tsi = layer->GetTileTilesetIndex(c,r);
	  // if tile has an index in tileset, then it can be drawn.
	  if ( tsi != -1 )
	  {
	    int tileWidth     = map->GetTileset(tsi)->GetTileWidth();
	    int tileHeight    = map->GetTileset(tsi)->GetTileHeight();
	    int tilesOnRow    = map->GetTileset(tsi)->GetImage()->GetWidth() / tileWidth;
	    // access our current tile
	    const Tmx::MapTile & tile = layer->GetTile(c,r);
	    // Compute tile position in tileset image.
	    // (tileset ids begin from 1, hence -1).
	    int tileColumn = tile.id  % tilesOnRow;
	    int tileRow    = tile.id  / tilesOnRow;

	    // Where to extract tile from tileset image.
	    SDL_Rect src = { tileColumn * tileWidth,
			     tileRow * tileHeight,
			     tileWidth,
			     tileHeight };

	    // Where to put that tile on screen.
	    SDL_Rect dest = { c * tileWidth, r * tileHeight,
			      tileWidth, tileHeight };
	    
	    SDL_RenderCopy( renderer, pBackgroundTiles, &src, &dest);
	  }
	} 
      }
    }
    
    // Changes to screen
    SDL_RenderPresent( renderer );
    SDL_Delay(1);
  }
  delete map;
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}
