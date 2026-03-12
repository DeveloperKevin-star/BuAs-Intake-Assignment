#pragma once
#include <vector>
#include <memory>
#include "SFML/Graphics.hpp"
#include "Enemy.h"
#include "Tower.h"
#include "Projectile.h"
#include "PathNode.h"
#include "WaveLogic.h"

struct InputState
{
    float mouseX = 0.0f;
    float mouseY = 0.0f;
    bool placeTowerPressed = false;
};

class Level
{
public:
    //game
    Level(const LevelConfig& config);

    void update(float dt);
    void render(sf::RenderWindow& window);

    void handlePlayerInput(const InputState& input);

    //tower
    bool placeTowerAt(float x, float y);
    bool canPlaceTowerAt(float x, float y) const;


    //Level
    bool isCompleted() const { return completed; }
    bool isFailed() const { return failed; }

    int getEcoSystemHealth() const { return ecoSystemHealth; }
    int getMoney() const { return money; }
    int getCurrentWaveIndex() const { return currentWaveIndex; }
    int getTowerCost() const { return towerCost; }

    static Level createCitySmogLevel();

private:
    //enemy functions
    void spawnEnemies(float dt);
    void updateEnemies(float dt);
    void drawEnemies(sf::RenderWindow& window);
    sf::Color getEnemyColor(EnemyType type) const; // this handles the enemy colors
    sf::Color getHealthBarColor(float healthPercent) const;
    void drawEnemyHealthbars(sf::RenderWindow& window, const Enemy& enemy);

    // tower functions
    void updateTowers(float dt);
    void updateProjectiles(float dt);
    void handleCollision();
    void drawTowers(sf::RenderWindow& window);
    void drawProjectiles(sf::RenderWindow& window);

    //path functions
    bool isTooCloseToPath(float x, float y) const;
    bool isTooCloseToTower(float x, float y) const;
    void drawPath(sf::RenderWindow& windwow);

    //config
    LevelConfig config;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<std::unique_ptr<Projectile>> projectiles;
    
    //waves
    float waveTimer = 0.0f;
    int currentWaveIndex = 0;
    int ecoSystemHealth = 0;

    //money
    int money = 100;
    int towerCost = 50;
    int killReward = 10;

    //level status
    bool completed = false;
    bool failed = false;
};