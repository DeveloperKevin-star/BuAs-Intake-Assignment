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
    Level(const LevelConfig& config);

    void update(float dt);
    void render(sf::RenderWindow& window);

    void handlePlayerInput(const InputState& input);

    bool placeTowerAt(float x, float y);
    bool canPlaceTowerAt(float x, float y) const;

    bool isCompleted() const { return completed; }
    bool isFailed() const { return failed; }

    int getEcoSystemHealth() const { return ecoSystemHealth; }
    int getMoney() const { return money; }
    int getCurrentWaveIndex() const { return currentWaveIndex; }
    int getTowerCost() const { return towerCost; }

    static Level createCitySmogLevel();

private:
    void spawnEnemies(float dt);
    void updateEnemies(float dt);
    void updateTowers(float dt);
    void updateProjectiles(float dt);
    void handleCollision();

    bool isTooCloseToPath(float x, float y) const;
    bool isTooCloseToTower(float x, float y) const;

    LevelConfig config;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<std::unique_ptr<Projectile>> projectiles;

    float waveTimer = 0.0f;
    int currentWaveIndex = 0;
    int ecoSystemHealth = 0;

    int money = 100;
    int towerCost = 50;
    int killReward = 10;

    bool completed = false;
    bool failed = false;
};