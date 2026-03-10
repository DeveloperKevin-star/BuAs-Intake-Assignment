#pragma once
#include <vector>
#include <memory>
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
    void render();

    bool isCompleted() const { return completed; }
    bool isFailed() const { return failed; }

    static Level createCitySmogLevel();

private:
    void spawnEnemies(float dt);
    void updateEnemies(float dt);
    void updateTowers(float dt);
    void updateProjectiles(float dt);
    void handleCollision();

    LevelConfig config;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<std::unique_ptr<Projectile>> projectiles;

    float waveTimer = 0.0f;
    int currentWaveIndex = 0;
    int ecoSystemHealth = 0;

    bool completed = false;
    bool failed = false;
};