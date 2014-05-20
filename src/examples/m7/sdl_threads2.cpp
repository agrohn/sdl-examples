////////////////////////////////////////////////////////////////////////////////
/// SDL Programming
////////////////////////////////////////////////////////////////////////////////
/// Thread example with main thread + two worker threads.
// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>
#include <iostream>
#include <stdexcept>
////////////////////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////////////////////
// Data passed to CreateThread must be an int pointer when this function is used.
int thread_function( void * data )
{
  int *pValue = reinterpret_cast<int *>(data);
  int prev;
  // process until data value becomes -1
  while( *pValue != -1 )
  {
    if ( *pValue != prev )
    {
      cout << "value changed to " << *pValue << endl;
      prev = *pValue;
    }
    else cerr << ".";
      
    SDL_Delay(100);
  }
  cout << "Thread one exiting" << endl;
  return 0;
}
////////////////////////////////////////////////////////////////////////////////
int thread_function_two( void *data)
{
  int *pValue = reinterpret_cast<int *>(data);
  // Decrement until -1  is reached
  while( *pValue != -1 )
  {
    (*pValue)--;
    cout << "Thread two changed value" << endl;
    SDL_Delay(1000);
  }
  cout << "Thread two exiting" << endl;
  return 5;
}
////////////////////////////////////////////////////////////////////////////////
int main()
{
  SDL_Thread *pThread;
  SDL_Thread *pThread2;

  int i = 10;
  int return_code;

  if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) throw runtime_error(SDL_GetError());
  // Create threads
  pThread = SDL_CreateThread( thread_function, "one", &i );
  pThread2 = SDL_CreateThread( thread_function_two, "two", &i ); 

  if ( pThread == NULL || pThread2 == NULL )
  {
    throw runtime_error(SDL_GetError());
  }

  // Clean up threads
  SDL_WaitThread( pThread, &return_code );
  cout << "Thread one exited with " << return_code << endl;
  SDL_WaitThread( pThread2, &return_code );
  cout << "Thread2 one exited with " << return_code << endl;

  // Clean up SDL
  SDL_Quit();
  return 0;
}
