////////////////////////////////////////////////////////////////////////////////
// SDL Programming
////////////////////////////////////////////////////////////////////////////////
// Example of deadlock situation with mutexes.
// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <iostream> 
#include <cstdlib>
#include <ctime>
#include <stdexcept>
////////////////////////////////////////////////////////////////////////////////
using namespace std;
////////////////////////////////////////////////////////////////////////////////
struct Data
{
  SDL_mutex *mutex_one;
  SDL_mutex *mutex_two;

  Data()
  {
    mutex_one = SDL_CreateMutex();
    mutex_two = SDL_CreateMutex();
  }
  ~Data()
  {
    SDL_DestroyMutex(mutex_one);
    SDL_DestroyMutex(mutex_two);
  }
};
////////////////////////////////////////////////////////////////////////////////
int thread_f_one( void * data )
{
  Data *pData = reinterpret_cast<Data *>(data);
  cout << SDL_ThreadID() << " : Locking mutex two..." << endl;
  SDL_LockMutex( pData->mutex_two );
  cout << SDL_ThreadID() << " : mutex two locked." << endl;

  SDL_Delay(100);

  cout << SDL_ThreadID() << " : Locking mutex one" << endl;
  SDL_LockMutex( pData->mutex_one );
  cout << SDL_ThreadID() << " : mutex one locked." << endl;

  cout << SDL_ThreadID() << " : Unlocking mutexes" << endl;
  SDL_UnlockMutex( pData->mutex_one );
  SDL_UnlockMutex( pData->mutex_two );

  return 0;
}
////////////////////////////////////////////////////////////////////////////////
int thread_f_two( void * data )
{
  Data *pData = reinterpret_cast<Data *>(data);
  cout << SDL_ThreadID() << " : Locking mutex one" << endl;
  SDL_LockMutex( pData->mutex_one );
  cout << SDL_ThreadID() << " : mutex one locked." << endl;

  SDL_Delay(100);

  cout << SDL_ThreadID() << " : Locking mutex two" << endl;
  SDL_LockMutex( pData->mutex_two );
  cout << SDL_ThreadID() << " : mutex two locked." << endl;
    
  cout << SDL_ThreadID() << " : Unlocking mutexes" << endl;
  SDL_UnlockMutex( pData->mutex_one );
  SDL_UnlockMutex( pData->mutex_two );
  
  return 0;
}
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv )
{
  
  Data data;

  SDL_Thread * one;
  SDL_Thread * two;
  if ( SDL_Init( SDL_INIT_EVERYTHING) < 0 ) 
    throw runtime_error(SDL_GetError());

  one = SDL_CreateThread( thread_f_one, "One", &data );
  two = SDL_CreateThread( thread_f_two, "Two", &data );
    
  SDL_WaitThread( one, NULL );
  SDL_WaitThread( two, NULL );

  SDL_Quit();
  return 0;
}

