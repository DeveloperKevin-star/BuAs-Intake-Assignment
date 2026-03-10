#include "Game.h"

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
    // Add keyboard/mouse handling here later.
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
}