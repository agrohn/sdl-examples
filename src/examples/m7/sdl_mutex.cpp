////////////////////////////////////////////////////////////////////////////////
// SDL Programming
////////////////////////////////////////////////////////////////////////////////
// Example of unprotected data access with threads.
// \author anssi.grohn@karelia.fi (c) 2014
////////////////////////////////////////////////////////////////////////////////
#include <SDL.h>    
#include <iostream> 
#include <cstdlib>
#include <ctime>
#include <sstream>

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


    // get current data value
    int tmp = pData->value;
    // increase it
    tmp++;
    // sleep for a while 
    SDL_Delay(rand()%1000);
    // set new value 
    cerr << "setting value to " << tmp << endl;
    pData->value = tmp;


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


  // Create threads
  for( int i=0;i<MAX_THREADS; i++)
  {
    stringstream tmp("valuesetter");
    tmp <<i;
    threads[i] = SDL_CreateThread( incrementer,tmp.str().c_str(), &data );
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

