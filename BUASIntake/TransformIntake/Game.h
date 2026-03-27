#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Level.h"
#include <memory>

//--- Here the gamestates are created ---
enum class GameState
{
    MainMenu,
    Playing,
    LevelTransition,
    Victory,
    Defeat
};

//--- Here all the constructers, variables and functions are created for the base game to run ---
class Game
{
public:
    //-- Base Functions --
    Game();
    bool init();
    void run();
    void quit() { running = false; }

private:
    //-- Base Functions --
    bool running = false;
    void handleInput();
    void update(float dt);
    void render();
    void setupLevels();


    //-- State Machine --
    GameState currentState = GameState::MainMenu;
    InputState inputState;


    //-- Butttons --
    sf::RectangleShape startButton;
    sf::RectangleShape exitButton;
    sf::RectangleShape retryButton;
    sf::RectangleShape menuButton;

    sf::Text startButtonText;
    sf::Text exitButtonText;
    sf::Text retryButtonText;
    sf::Text menuButtonText;

    void setupButtons();
    bool isMouseOverButton(const sf::RectangleShape& button) const;
    void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);


    //-- Level Transistion --
    sf::Text transitionText;
    std::vector<std::unique_ptr<Level>> levels;
    bool isTransitioning = false;
    float transistionTimer = 0.f; 
    float transitionDuration = 2.f; 
    int currentLevelIndex = 0;
    int carryoverMoney = 0;
    

    //-- Rendering --
    sf::RenderWindow window;
    sf::Font font;
    sf::Text hudText;
    sf::Text waveText;


    //Menu Members
    void setMainMenuBackground();
    sf::Text titleText;
    sf::Text menuText;
    sf::Text endScreenText;
    sf::Texture menuBackgroundTexture;
    sf::Sprite menuBackgroundSprite;

    //Victory Memebers
    void setVictoryBackground();
    sf::Texture victoryBackgroundTexture;
    sf::Sprite victoryBackgroundSprite;
};