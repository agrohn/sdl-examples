#include <SDL.h>    
#include <iostream> 
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <stdexcept>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
struct Data
{
  SDL_mutex *mutex;
  bool running;
  int value;

  Data()
  {
    mutex = SDL_CreateMutex();
    running = true;
    value = 0;
  }
  ~Data()
  {
    SDL_DestroyMutex(mutex);
  }
};
////////////////////////////////////////////////////////////////////////////////
int incrementer( void * data )
{
  Data *pData = reinterpret_cast<Data *>(data);
  while( pData->running )
  {
    // lock access to data
    SDL_LockMutex( pData->mutex );
    // get current data value
    int tmp = pData->value;
    // increase it
    tmp++;
    // sleep for a while 
    SDL_Delay(rand()%1000);
    // set new value 
    cerr << "setting value to " << tmp << endl;
    pData->value = tmp;
    // release access to data
    SDL_UnlockMutex( pData->mutex );
  }
  return 0;
}
////////////////////////////////////////////////////////////////////////////////
#define MAX_THREADS 10
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv )
{
  
  Data data;

  srand(time(NULL));
  SDL_Thread *threads[MAX_THREADS];

  if ( SDL_Init( SDL_INIT_EVERYTHING) < 0 ) 
    throw runtime_error(SDL_GetError());
  
  // Create threads
  for( int i=0;i<MAX_THREADS; i++)
  {
    stringstream ss;
    ss << i;
    threads[i] = SDL_CreateThread( incrementer, ss.str().c_str(), &data );
    if ( threads[i] == NULL ) 
    {
      cout << "Cannot create thread " << endl;
      return 1;
    }
  }
  

  
  SDL_Delay(1000);
  data.running = 0;
  
  // join threads
  for( int i=0;i<MAX_THREADS; i++)
    SDL_WaitThread( threads[i], NULL );


  // print final value
  cout << "final value : " << data.value << endl;

  SDL_Quit();
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// SDL Programming 2010
////////////////////////////////////////////////////////////////////////////////
// Example of protected data access with threads.
// \author anssi.grohn@pkamk.fi
////////////////////////////////////////////////////////////////////////////////
