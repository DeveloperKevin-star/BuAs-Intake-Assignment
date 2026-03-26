#include "Game.h"
#include <windows.h>
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
    setupLevels();


    if (!font.loadFromFile("assets/fonts/ARIAL.TTF"))
        return false;

    //buttons
    setupButtons();

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

    //Trans text
    transitionText.setFont(font);
    transitionText.setCharacterSize(32);
    transitionText.setFillColor(sf::Color::White);
    transitionText.setPosition(400.f, 300.f);

    //Title Screen Text
    titleText.setFont(font);
    titleText.setCharacterSize(42);
    titleText.setFillColor(sf::Color::White);
    titleText.setString("Eco Tower Defence");
    titleText.setPosition(350.f, 180.f);
    
    //Menu text
    menuText.setFont(font);
    menuText.setCharacterSize(24);
    menuText.setFillColor(sf::Color::White);
    menuText.setString("Press Enter to Start");
    menuText.setPosition(420.f, 300.f);

    //End screen text
    endScreenText.setFont(font);
    endScreenText.setCharacterSize(32);
    endScreenText.setFillColor(sf::Color::White);
    endScreenText.setPosition(350.f, 260.f);
    
    //buttons


    currentState = GameState::MainMenu;
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
        {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            if (currentState == GameState::MainMenu)
            {
                if (isMouseOverButton(startButton))
                {
                    setupLevels();
                    carryoverMoney = 0;
                    currentState = GameState::Playing;
                }
                else if (isMouseOverButton(exitButton))
                {
                    window.close();
                }
            }
            else if (currentState == GameState::Victory)
            {
                if (isMouseOverButton(menuButton))
                {
                    currentState = GameState::MainMenu;
                }
                else if (isMouseOverButton(exitButton))
                {
                    window.close();
                }
            }
            else if (currentState == GameState::Defeat)
            {
                if (isMouseOverButton(retryButton))
                {
                    setupLevels();
                    currentState = GameState::Playing;
                }
                else if (isMouseOverButton(menuButton))
                {
                    currentState = GameState::MainMenu;
                }
                else if (isMouseOverButton(exitButton))
                {
                    window.close();
                }
            }
            else if (currentState == GameState::Playing)
            {
                inputState.placeTowerPressed = true;
            }
        }

        if (currentState == GameState::Playing &&
            event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
            inputState.placeTowerPressed = true;
    }

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    inputState.mouseX = static_cast<float>(mousePos.x);
    inputState.mouseY = static_cast<float>(mousePos.y);
}

void Game::update(float dt)
{
    // The game has gamestates to handle it better for the player in the menu's, and now certain stuff only updates when they are needed to
    //switch (currentState)
    //{
    //    case GameState::MainMenu:
    //    break;

    //    case GameState::Playing:
    //    {
    //        if (currentLevelIndex >= static_cast<int>(levels.size()))
    //            return;

    //        Level& lvl = *levels.at(currentLevelIndex);
    //        lvl.handlePlayerInput(inputState);
    //        lvl.update(dt);

    //        if (lvl.isFailed())
    //        {
    //            endScreenText.setString("Defeat!\nPress Enter for Main Menu.");
    //            currentState = GameState::Defeat;
    //            return;
    //        }

    //        if (lvl.isCompleted())
    //        {
    //            OutputDebugStringA("Entered Level transistion\n");
    //            carryoverMoney = lvl.getMoney();
    //            currentState = GameState::LevelTransition;
    //            transistionTimer = transitionDuration;
    //           

    //            if (currentLevelIndex + 1 < static_cast<int>(levels.size()))
    //            {
    //                transitionText.setString("LevelComplete!\n\nSavedMoney: " + std::to_string(carryoverMoney) +
    //                    "\nNext: " + levels.at(currentLevelIndex + 1)->getName() + ".");
    //            }
    //            else
    //            {
    //                transitionText.setString("Final Level Complete!");
    //            }

    //            return;
    //        }
    //        break;
    //    }
    //    case GameState::LevelTransition:
    //        transistionTimer -= dt;
    //        if (transistionTimer <= 0.f)
    //        {
    //            OutputDebugStringA("TransitionTimer finished \n");
    //            ++currentLevelIndex;

    //            if (currentLevelIndex >= static_cast<int>(levels.size()))
    //            {
    //                OutputDebugStringA("Entered Victory State\n");
    //                endScreenText.setString("Victory!\nPress Enter for Main Menu.");
    //                currentState = GameState::Victory;
    //            }
    //            else
    //            {
    //                OutputDebugStringA("Back tro playing State\n");
    //                levels.at(currentLevelIndex)->addMoney(carryoverMoney);
    //                currentState = GameState::Playing;
    //            }
    //            return;
    //        }
    //        break;

    //    //Both victory and defeat will wait for the enter input
    //    case GameState::Victory:
    //        break;

    //    case GameState::Defeat:
    //        break;
    //}

    switch (currentState)
    {
    case GameState::MainMenu:
        break;

    case GameState::Playing:
    {
        if (currentLevelIndex >= static_cast<int>(levels.size()))
        {
            OutputDebugStringA("Playing: currentLevelIndex out of range\n");
            return;
        }

        Level& lvl = *levels.at(currentLevelIndex);
        lvl.handlePlayerInput(inputState);
        lvl.update(dt);

        if (lvl.isFailed())
        {
            OutputDebugStringA("Entered Defeat\n");
            endScreenText.setString("Defeat!\nPress Enter for Main Menu.");
            currentState = GameState::Defeat;
            return;
        }

        if (lvl.isCompleted())
        {
            std::string msg = "Entered LevelTransition from level index " + std::to_string(currentLevelIndex) + "\n";
            OutputDebugStringA(msg.c_str());

            carryoverMoney = lvl.getMoney();
            currentState = GameState::LevelTransition;
            transistionTimer = transitionDuration;

            if (currentLevelIndex + 1 < static_cast<int>(levels.size()))
            {
                transitionText.setString(
                    "LevelComplete!\n\nSavedMoney: " + std::to_string(carryoverMoney) +
                    "\nNext: " + levels.at(currentLevelIndex + 1)->getName() + "."
                );
            }
            else
            {
                transitionText.setString("Final Level Complete!");
            }

            return;
        }

        break;
    }

    case GameState::LevelTransition:
    {
        transistionTimer -= dt;

        std::string msg = "LevelTransition: timer=" + std::to_string(transistionTimer) +
            " currentLevelIndex=" + std::to_string(currentLevelIndex) +
            " levels.size=" + std::to_string(levels.size()) + "\n";
        OutputDebugStringA(msg.c_str());

        if (transistionTimer <= 0.f)
        {
            ++currentLevelIndex;

            std::string msg2 = "Transition finished: currentLevelIndex now " + std::to_string(currentLevelIndex) + "\n";
            OutputDebugStringA(msg2.c_str());

            if (currentLevelIndex >= static_cast<int>(levels.size()))
            {
                OutputDebugStringA("Entered Victory state\n");
                endScreenText.setString("Victory!\nPress Enter for Main Menu.");
                currentState = GameState::Victory;
            }
            else
            {
                OutputDebugStringA("Returning to Playing\n");
                levels.at(currentLevelIndex)->addMoney(carryoverMoney);
                currentState = GameState::Playing;
            }
            return;
        }
        break;
    }

    case GameState::Victory:
        OutputDebugStringA("Currently in Victory\n");
        break;

    case GameState::Defeat:
        break;
    }

}

void Game::render()
{
    window.clear(sf::Color(20, 20, 20));

    startButton.setFillColor(
        isMouseOverButton(startButton) ? sf::Color(100, 160, 210) : sf::Color(70, 130, 180)
    );

    exitButton.setFillColor(
        isMouseOverButton(exitButton) ? sf::Color(210, 90, 90) : sf::Color(180, 70, 70)
    );

    switch (currentState)
    {
        case GameState::MainMenu:
        {
            window.draw(titleText);
            window.draw(startButton);
            window.draw(startButtonText);
            window.draw(exitButton);
            window.draw(exitButtonText);
            break;
        }

        case GameState::Playing:
            {
                if (currentLevelIndex >= 0 && currentLevelIndex < static_cast<int>(levels.size()))
            {
                Level& lvl = *levels.at(currentLevelIndex);
                lvl.render(window);

                bool canPlace = lvl.canPlaceTowerAt(inputState.mouseX, inputState.mouseY);

                sf::CircleShape rangePreview(100.0f);
                rangePreview.setOrigin(100.f, 100.f);
                rangePreview.setPosition(inputState.mouseX, inputState.mouseY);
                rangePreview.setFillColor(sf::Color(0, 0, 0, 0));
                rangePreview.setOutlineThickness(2.0f);
                rangePreview.setOutlineColor(
                    canPlace ? sf::Color(0, 255, 0, 120)
                    : sf::Color(255, 0, 0, 120)
                );
                window.draw(rangePreview);

                sf::CircleShape towerPreview(15.f);
                towerPreview.setOrigin(15.f, 15.f);
                towerPreview.setPosition(inputState.mouseX, inputState.mouseY);
                towerPreview.setFillColor(
                    canPlace ? sf::Color(0, 255, 0, 150)
                    : sf::Color(255, 0, 0, 150)
                );
                window.draw(towerPreview);

                std::string hud =
                    "Health: " + std::to_string(lvl.getEcoSystemHealth()) +
                    "   Money: " + std::to_string(lvl.getMoney()) +
                    "   Wave: " + std::to_string(lvl.getCurrentWaveIndex()) +
                    "   Tower Cost: " + std::to_string(lvl.getTowerCost());

                hudText.setString(hud);
                window.draw(hudText);

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
                break;
            }

        case GameState::LevelTransition:
            {
                if (currentLevelIndex >= 0 && currentLevelIndex < static_cast<int>(levels.size()))
                {
                    levels.at(currentLevelIndex)->render(window);
                }

                window.draw(transitionText);
                break;
            }

        case GameState::Victory:
            {
                window.draw(titleText);
                window.draw(menuButton);
                window.draw(menuButtonText);
                window.draw(exitButton);
                window.draw(exitButtonText);
                window.draw(endScreenText);
                break;
            }

        case GameState::Defeat:
            {
                window.draw(titleText);
                window.draw(endScreenText);
                window.draw(retryButton);
                window.draw(retryButtonText);
                window.draw(menuButton);
                window.draw(menuButtonText);
                window.draw(exitButton);
                window.draw(exitButtonText);
                break;
            }
    }
    window.display();
}

void Game::setupLevels() {
    levels.clear();
    levels.emplace_back(std::make_unique<Level>(Level::createCitySmogLevel()));
    levels.emplace_back(std::make_unique<Level>(Level::createIndustrialLevel()));
    levels.emplace_back(std::make_unique<Level>(Level::createHarborLevel()));
    levels.emplace_back(std::make_unique<Level>(Level::createToxicLevel()));
    levels.emplace_back(std::make_unique<Level>(Level::createMFactoryLevel()));

    currentLevelIndex = 0;
}

void Game::setupButtons() {
    // Start button
    startButton.setSize(sf::Vector2f(220.f, 60.f));
    startButton.setPosition(530.f, 280.f);
    startButton.setFillColor(sf::Color(70, 130, 180));

    startButtonText.setFont(font);
    startButtonText.setCharacterSize(24);
    startButtonText.setFillColor(sf::Color::White);
    startButtonText.setString("Start Game");
    centerTextInButton(startButtonText, startButton);


    // Exit button
    exitButton.setSize(sf::Vector2f(220.f, 60.f));
    exitButton.setPosition(530.f, 370.f);
    exitButton.setFillColor(sf::Color(180, 70, 70));

    exitButtonText.setFont(font);
    exitButtonText.setCharacterSize(24);
    exitButtonText.setFillColor(sf::Color::White);
    exitButtonText.setString("Exit");
    centerTextInButton(exitButtonText, exitButton);


    // Retry button
    retryButton.setSize(sf::Vector2f(220.f, 60.f));
    retryButton.setPosition(530.f, 320.f);
    retryButton.setFillColor(sf::Color(70, 160, 70));

    retryButtonText.setFont(font);
    retryButtonText.setCharacterSize(24);
    retryButtonText.setFillColor(sf::Color::White);
    retryButtonText.setString("Retry");
    centerTextInButton(retryButtonText, retryButton);


    // Menu button
    menuButton.setSize(sf::Vector2f(220.f, 60.f));
    menuButton.setPosition(530.f, 410.f);
    menuButton.setFillColor(sf::Color(100, 100, 180));

    menuButtonText.setFont(font);
    menuButtonText.setCharacterSize(24);
    menuButtonText.setFillColor(sf::Color::White);
    menuButtonText.setString("Main Menu");
    centerTextInButton(menuButtonText, menuButton);
}

void Game::centerTextInButton(sf::Text& text, const sf::RectangleShape& button)
{
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::FloatRect buttonBounds = button.getGlobalBounds();

    text.setOrigin(
        textBounds.left + textBounds.width / 2.f,
        textBounds.top + textBounds.height / 2.f
    );

    text.setPosition(
        buttonBounds.left + buttonBounds.width / 2.f,
        buttonBounds.top + buttonBounds.height / 2.f
    );
}

bool Game::isMouseOverButton(const sf::RectangleShape& button) const
{
    sf::Vector2 mousePos(inputState.mouseX, inputState.mouseY);
    return button.getGlobalBounds().contains(mousePos);
}