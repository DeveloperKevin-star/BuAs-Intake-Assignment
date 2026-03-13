#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
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
};