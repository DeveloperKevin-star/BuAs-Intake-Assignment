#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Level.h"
#include <memory>

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

    void setMainMenuBackground();
    void setVictoryBackground();
    

    //State Machine
    GameState currentState = GameState::MainMenu;
    InputState inputState;


    //Butttons
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


    // Level Transistion
    bool isTransitioning = false;
    float transistionTimer = 0.f; 
    float transitionDuration = 2.f; 
    sf::Text transitionText;

    int carryoverMoney = 0;
    

    // rendering
    sf::RenderWindow window;

    sf::Font font;
    sf::Text hudText;
    sf::Text waveText;

    bool running = false;
    std::vector<std::unique_ptr<Level>> levels;
    int currentLevelIndex = 0;


    //Menu Members
    sf::Text titleText;
    sf::Text menuText;
    sf::Text endScreenText;
    sf::Texture menuBackgroundTexture;
    sf::Sprite menuBackgroundSprite;

    //Victory Memebers
    sf::Texture victoryBackgroundTexture;
    sf::Sprite victoryBackgroundSprite;
};