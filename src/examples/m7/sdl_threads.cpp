////////////////////////////////////////////////////////////////////////////////
/// SDL Programming 
////////////////////////////////////////////////////////////////////////////////
/// A simple thread example.
/// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>
#include <iostream>
#include <stdexcept>
////////////////////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////////////////////
// data must be an int pointer
int thread_function( void * data )
{
  int *pValue = reinterpret_cast<int *>(data);
  int prev = *pValue;
  // process until data value becomes -1
  while( *pValue != -1 )
  {
    if ( *pValue != prev )
    {
      cout << "*Thread* value changed to " << *pValue << endl;
      prev = *pValue;
    }
    SDL_Delay(100);
  }
  cout << "*Thread* exiting" << endl;
  return 0;
}
////////////////////////////////////////////////////////////////////////////////
int main()
{
  SDL_Thread *pThread;
  int i;
  int return_code;
  
  if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) throw runtime_error(SDL_GetError());
  
  pThread = SDL_CreateThread( thread_function, "increment", &i );
  
  if ( pThread == NULL )
  {
    cout << "*Main* Cannot create thread " << SDL_GetError() << endl;
    return 1;
  }

  // During five seconds, increment value of variable i five times
  for( i=0; i<5;++i)
  {
    cout << "*Main* Changing value to " << i << endl;
    SDL_Delay(1000);
  }
  
  cout << "*Main* Sending thread exit signal" << endl;
  i = -1;

  SDL_WaitThread( pThread, &return_code );
  cout << "*Main* Thread exited with " << return_code << endl;
  SDL_Quit();
  return 0;
}
