/////////////////////////////////////////////////////////////////
/// SDL Programming
////////////////////////////////////////////////////////////////////////////////
/// A simple number guessing game, within a class.
/// \author anssi.grohn@karelia.fi (c) 2014
/////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
/////////////////////////////////////////////////////////////////
using namespace std;
/////////////////////////////////////////////////////////////////
const int MAX_GUESSES = 2;

class NumberGuessGame
{
public:
  bool running_;

  unsigned int numGuesses;
  int userGuess_;
  int correctNumber_;
  bool guessed_;
  int numGuesses_;

  NumberGuessGame()
  {
    running_ = true;
    numGuesses_ = 0;
    userGuess_ = 0;
    correctNumber_ = 0;
    guessed_ = false;
  }

  virtual ~NumberGuessGame()
  {
    
  }
  
  void Play()
  {
    string input;
    cout << "Number guessing game v.1.0." << endl;
    cout << " Would you like to play a game (y/n)?\n> ";
    cin >> input;
  
    if ( input[0] == 'y' ) running_ = true;
    else running_ = false;

    while( running_ )
    {
    
      cout << "You have " << MAX_GUESSES << " tries to guess what number I am thinking. " << endl ;
      correctNumber_ = rand() % 10;
      guessed_ = false;
      numGuesses_ = 0;
      while( numGuesses_ < MAX_GUESSES && !guessed_ )
      {
	cout << "Guess #" << numGuesses_+1 << ": ";
	cin >> input;
	stringstream(input) >> userGuess_;

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
	}
	++numGuesses_;
      }
      
      if ( guessed_ == false)
      {
	cout << "Game Over!" << endl;
	cout << "You did not guess my number, which was " << correctNumber_ << endl;
      }

      cout << "Would you like to play again (y/n)? : ";
      cin >> input;
      running_ = ( input[0] == 'y' );
    }
    cout << "Byebye" << endl;
  }
};
////////////////////////////////////////////////////////////////////////////////
int main()
{

  NumberGuessGame game;
  game.Play();

  return 0;
}
/////////////////////////////////////////////////////////////////
