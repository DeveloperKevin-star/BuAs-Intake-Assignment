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

    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            inputState.placeTowerPressed = true;
        }
    }

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    inputState.mouseX = static_cast<float>(mousePos.x);
    inputState.mouseY = static_cast<float>(mousePos.y);
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
    window.clear(sf::Color(20, 20, 20));

    if (currentLevelIndex >= 0 && currentLevelIndex < static_cast<int>(levels.size()))
    {
        Level& lvl = levels[currentLevelIndex];
        lvl.render(window);

        bool canPlace = lvl.canPlaceTowerAt(inputState.mouseX, inputState.mouseY);

        // preview drawing can go here later using canPlace
    }

    window.display();
}