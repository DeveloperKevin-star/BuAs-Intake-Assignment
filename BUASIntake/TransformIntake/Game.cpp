#include "Game.h"

#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-system-d.lib")



//this handles the whole game, while the level class handles the levels
Game::Game() 
    : window(sf::VideoMode(1280, 720), "Eco Tower Defence")
{
    window.setFramerateLimit(60);
}

bool Game::init()
{
    levels.emplace_back(Level::createCitySmogLevel());

    if (!font.loadFromFile("ARIAL.TTF"))
        return false;
    // hud text
    hudText.setFont(font);
    hudText.setCharacterSize(20);
    hudText.setFillColor(sf::Color::White);
    hudText.setPosition(10.f, 10.f);

    //wave text
    waveText.setFont(font);
    waveText.setCharacterSize(18);
    waveText.setFillColor(sf::Color::Cyan);
    waveText.setPosition(10.f, 40.f);

    running = true;
    return true;
}

void Game::run()
{
    sf::Clock clock;

    while (running && window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        if (dt > 0.5f)
            dt = 0.5f;

        handleInput();
        update(dt);
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

    //Level& lvl = levels[currentLevelIndex];
    Level& lvl = levels.at(currentLevelIndex);
    lvl.handlePlayerInput(inputState);
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

        //This draws the circle preview for a tower
        sf::CircleShape rangePreview(100.0f);
        rangePreview.setOrigin(100.f, 100.f);
        rangePreview.setPosition(inputState.mouseX, inputState.mouseY);
        rangePreview.setFillColor(sf::Color(0, 0, 0, 0));
        rangePreview.setOutlineThickness(2.0f);

        if (canPlace) 
        {
            rangePreview.setOutlineColor(sf::Color(0, 255, 0, 120));
        }
        else
        {
            rangePreview.setOutlineColor(sf::Color(255, 0, 0, 120));
        }

        //this draws the tower ghost preview
        sf::CircleShape towerPreview(15.f);
        towerPreview.setOrigin(15.f, 15.f);
        towerPreview.setPosition(inputState.mouseX, inputState.mouseY);

        if (canPlace)
        {
            towerPreview.setFillColor(sf::Color(0, 255, 0, 150));
        }
        else
        {
            towerPreview.setFillColor(sf::Color(255, 0, 0, 150));
        }

        window.draw(towerPreview);

        //Here is the hud drawn on the screen so the player sees it
        std::string hud =
            "Health: " + std::to_string(lvl.getEcoSystemHealth()) +
            "   Money: " + std::to_string(lvl.getMoney()) +
            "   Wave: " + std::to_string(lvl.getCurrentWaveIndex()) +
            "   Tower Cost: " + std::to_string(lvl.getTowerCost()) +
            "   WaveCountdown: " + std::to_string(lvl.getNextSpawnCountdown());


        hudText.setString(hud);

        window.draw(hudText);

        //This is the wave countdown text
        if (lvl.hasMoreWaves())
        {
            float countdown = lvl.getNextSpawnCountdown();
            waveText.setString("Next Enemy in: " + std::to_string(countdown).substr(0, 4) + "s");
        }
        else 
        {
            waveText.setString("All waves spawned");
        }

        window.draw(waveText);
    }

    window.display();
}