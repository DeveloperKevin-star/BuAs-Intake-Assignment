#include "Game.h"


//this handles the whole game, while the level class handles the levels
Game::Game() {}

bool Game::init()
{
    levels.emplace_back(Level::createCitySmogLevel());
    running = true;
    return true;
}

void Game::run()
{
    const float fixedDt = 1.0f / 60.0f;

    while (running)
    {
        handleInput();
        update(fixedDt);
        render();
    }
}

void Game::handleInput()
{
    inputState.placeTowerPressed = false;

    // Add keyboard/mouse handling here later.


    inputState.mouseX = mouseX;
    inputState.mouseY = mouseY;
}

void Game::update(float dt)
{
    if (currentLevelIndex < 0 || currentLevelIndex >= static_cast<int>(levels.size()))
        return;

    Level& lvl = levels[currentLevelIndex];
    lvl.update(dt);

    if (lvl.isFailed())
    {
        quit();
        return;
    }

    if (lvl.isCompleted())
    {
        ++currentLevelIndex;
        if (currentLevelIndex >= static_cast<int>(levels.size()))
        {
            quit();
        }
    }
}

void Game::render()
{
    if (currentLevelIndex >= 0 && currentLevelIndex < static_cast<int>(levels.size()))
    {
        levels[currentLevelIndex].render();
    }
    
    if (inputState.placeTowerPressed)
    {
        level.tryPlaceTower(mouseX, mouseY);
    }
}