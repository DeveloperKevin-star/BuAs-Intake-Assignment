#pragma once
#include <vector>
#include "Level.h"

//this is base game
class Game
{
public:
    Game();
    bool init();
    void run();
    void quit() { running = false; }

private:
    void handleInput();
    void update(float dt);
    void render();

    bool running = false;
    std::vector<Level> levels;
    int currentLevelIndex = 0;
};