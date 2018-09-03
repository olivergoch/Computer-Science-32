#ifndef GAME_H
#define GAME_H


#include "Arena.h"
#include <string>

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nRats);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
    bool   m_wasDisplay;
    
    // Helper functions
    std::string takePlayerTurn();
};


#endif /* defined(___2Project_1__Game__) */
