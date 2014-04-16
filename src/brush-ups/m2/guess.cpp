/////////////////////////////////////////////////////////////////
/// SDL Programming
////////////////////////////////////////////////////////////////////////////////
/// A simple number guessing game, within a class. Altered so, that input is
/// read from SDL events.
/// \author anssi.grohn@karelia.fi (c) 2014
/////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <SDLApp.h>

/////////////////////////////////////////////////////////////////
using namespace std;
/////////////////////////////////////////////////////////////////
const int MAX_GUESSES = 2;
// Game states ,used to check which input will be required at which stage.
enum State { ASK_START, GAME_ON, EXIT };
////////////////////////////////////////////////////////////////////////////////
// inherit from SDLApp 
class NumberGuessGame : public SDLApp
{
public:

  int userGuess_; 
  int correctNumber_;
  bool guessed_;
  int numGuesses_;
  State state_;

  NumberGuessGame()
  {
    numGuesses_ = 0;
    userGuess_ = 0;
    correctNumber_ = 0;
    guessed_ = false;
    state_ = ASK_START;
  }
  
  virtual ~NumberGuessGame()
  {
    
  }
  
  void Play()
  {
    // initialize SDL window and renderer
    Init();

    cout << "Number guessing game v.1.0." << endl;
    cout << " Would you like to play a game (y/n)?\n> ";
    // main loop
    while( state_ != EXIT )
    {
      HandleInput();
      Render();
    }
    cout << "Byebye" << endl;
  }

  // Overwritten key press handler function.
  void OnKeyPress(SDL_Keycode key) /* override */
  {

    if ( state_ == ASK_START )
    {
      // ask input for starting or continue.
      switch( key )
      {
      case SDLK_y:
	state_ = GAME_ON;
	cout << "You have " << MAX_GUESSES 
	     << " tries to guess what number I am thinking. " << endl ;
	correctNumber_ = rand() % 10;
	guessed_ = false;
	numGuesses_ = 0;
	break;
      case SDLK_n:
	state_ = EXIT;
	break;
      default:
	break;
      }
    }
    else if  ( state_ == GAME_ON )
    {
      // in game, ask numbers 0-9.
      if ( numGuesses_ < MAX_GUESSES && !guessed_ )
      {
	cout << "Guess #" << numGuesses_+1 << ": ";
	cout.flush();
	if ( key >= SDLK_0 && key <= SDLK_9 )
	{
	  userGuess_ = 0 + (SDLK_9-key);
	}
	else if ( key >= SDLK_KP_0 && key <= SDLK_KP_9 )
	{
	  userGuess_ = 0 + (SDLK_KP_9-key);
	}
	else 
	{
	  cout << "please input values between 0-9 only\n";
	  userGuess_ = -1;
	}

	if ( userGuess_ > -1 )
	{
	  if ( userGuess_ < correctNumber_ )
	  {
	    cout << "Your guess was too small. Try again." << endl;
	  }
	  else if ( userGuess_ > correctNumber_ )
	  {
	    cout << "Your guess was too large. Try again." << endl; 
	  }
	  else
	  {
	    cout << "Congratulations, you guessed the number by  "
		 << numGuesses_+1 << " guesses" << endl;
	    guessed_ = true;
	    cout << "Would you like to play again (y/n)? : ";

	    state_ = ASK_START;
	  }
	  cout.flush();
	  ++numGuesses_;
	}
      }
      // Game over check.
      if ( numGuesses_ >= MAX_GUESSES && guessed_ == false)
      {
	cout << "Game Over!" << endl;
	cout << "You did not guess my number, which was " 
	     << correctNumber_ << endl;
	cout << "Would you like to play again (y/n)? : ";
	cout.flush();
	state_ = ASK_START;
      }
    }
  }
  // Overwritten quit handler function.
  void OnQuit() /* override */
  {
    state_ = EXIT;
  }

};
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
  NumberGuessGame game;
  game.Play();
  return 0;
}
/////////////////////////////////////////////////////////////////
