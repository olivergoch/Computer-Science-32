// rats.cpp

#include "Rat.h"
#include "Player.h"
#include "Arena.h"
#include "globals.h"
#include "Game.h"
#include "History.h"
using namespace std;

int main()
{
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(10, 12, 40);
    
    // Play the game
    g.play();
}
