#include "Game.h"
//--- The only function of this is creating the main function of the game --- 
int main()
{
    Game game;

    if (!game.init())
        return 1;

    game.run();
    return 0;
}