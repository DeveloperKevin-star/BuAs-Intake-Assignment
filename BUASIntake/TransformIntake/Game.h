#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Level.h"

// these are the game state for the state machines
enum class GameState
{
    MainMenu,
    Playing,
    LevelTransition,
    Victory,
    Defeat
};

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
    void setupLevels();

    GameState currentState = GameState::MainMenu;

    // Level Transistion
    bool isTransitioning = false;
    float transistionTimer = 0.f; 
    float transitionDuration = 2.f; 
    sf::Text transitionText;

    // rendering
    sf::RenderWindow window;

    sf::Font font;
    sf::Text hudText;
    sf::Text waveText;

    bool running = false;
    std::vector<Level> levels;
    int currentLevelIndex = 0;

    InputState inputState;

    //Menu Members
    sf::Text titleText;
    sf::Text menuText;
    sf::Text endScreenText;
};