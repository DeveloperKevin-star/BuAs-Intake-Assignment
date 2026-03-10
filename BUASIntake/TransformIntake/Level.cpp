#include "Level.h"
#include <cmath>


Level::Level(const LevelConfig& cfg)
    : config(cfg), ecoSystemHealth(cfg.initialHealth)
{
    // Keep or remove this starter tower.
    towers.push_back(std::make_unique<Tower>(150.0f, 120.0f, 100.0f, 1.0f, 3));
}

void Level::update(float dt)
{
    if (completed || failed)
        return;

    spawnEnemies(dt);
    updateEnemies(dt);
    updateTowers(dt);
    handleCollision();

    if (ecoSystemHealth <= 0)
    {
        failed = true;
    }
    else if (currentWaveIndex >= static_cast<int>(config.waves.size()) && enemies.empty())
    {
        completed = true;
    }
}

void Level::handlePlayerInput(const InputState& input)
{
    if (input.placeTowerPressed)
    {
        placeTowerAt(input.mouseX, input.mouseY);
    }
}

// This checks if a player wants to place a tower on a correct place, if they have enough money and if they do have enough it will place the tower and substract cost
bool Level::placeTowerAt(float x, float y)
{
    if (!canPlaceTowerAt(x, y))
        return false;

    if (money < towerCost)
        return false;

    towers.push_back(std::make_unique<Tower>(x, y, 100.0f, 1.0f, 3));
    money -= towerCost;
    return true;
}

//This checks whether a tower can be placed at said location
bool Level::canPlaceTowerAt(float x, float y) const
{
    if (money < towerCost)
        return false;

    if (isTooCloseToPath(x, y))
        return false;

    if (isTooCloseToTower(x, y))
        return false;

    return true;
}

//This prevents the player from placing the tower on the path
bool Level::isTooCloseToPath(float x, float y) const
{
    const float minDistanceFromPath = 40.0f;

    for (const PathNode& node : config.enemyPath)
    {
        float dx = node.x - x;
        float dy = node.y - y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist < minDistanceFromPath)
            return true;
    }

    return false;
}

bool Level::isTooCloseToTower(float x, float y) const
{
    const float minTowerSpacing = 50.0f;

    for (const auto& tower : towers)
    {
        float dx = tower->getX() - x;
        float dy = tower->getY() - y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist < minTowerSpacing)
            return true;
    }

    return false;
}

bool Level::canPlaceTowerAt(float x, float y)
{
    if (!canPlaceTowerAt(x, y))
        return false;
    
    towers.push_back(std::make_unique<Tower>(x, y, 100.0f, 1.0f, 3));
    money -= towerCost;
    return true;
}

void Level::spawnEnemies(float dt)
{
    if (currentWaveIndex >= static_cast<int>(config.waves.size()))
        return;

    waveTimer += dt;
    WaveConfig& wave = config.waves[currentWaveIndex];

    while (!wave.enemiesToSpawn.empty() && waveTimer >= wave.spawnInterval)
    {
        waveTimer -= wave.spawnInterval;

        EnemyType type = wave.enemiesToSpawn.front();
        wave.enemiesToSpawn.erase(wave.enemiesToSpawn.begin());

        enemies.push_back(std::make_unique<Enemy>(type, config.enemyPath));

        if (wave.enemiesToSpawn.empty())
        {
            ++currentWaveIndex;
            waveTimer = 0.0f;
        }
    }
}

void Level::updateEnemies(float dt)
{
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        (*it)->update(dt);

        if ((*it)->hasReachedGoal())
        {
            ecoSystemHealth -= (*it)->getPollutionDamage();
            it = enemies.erase(it);
        }
        else if (!(*it)->isAlive())
        {
            money += killReward;
            it = enemies.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Level::updateTowers(float dt)
{
    for (auto& tower : towers)
    {
        tower->update(dt, enemies);
    }
}

void Level::handleCollision()
{
    // Reserved for projectile logic later.
}

void Level::render()
{
    // Hook to your rendering library later.
}

Level Level::createCitySmogLevel()
{
    LevelConfig cfg;
    cfg.name = "City Smog Defence";
    cfg.initialHealth = 50;

    cfg.enemyPath = {
        {0.0f, 100.0f},
        {200.0f, 100.0f},
        {200.0f, 250.0f},
        {500.0f, 250.0f}
    };

    WaveConfig wave1;
    wave1.spawnInterval = 1.0f;
    wave1.enemiesToSpawn = {
        EnemyType::Smog,
        EnemyType::Smog,
        EnemyType::Plastic,
        EnemyType::Smog
    };

    WaveConfig wave2;
    wave2.spawnInterval = 0.75f;
    wave2.enemiesToSpawn = {
        EnemyType::Plastic,
        EnemyType::Plastic,
        EnemyType::Oil
    };

    cfg.waves.push_back(wave1);
    cfg.waves.push_back(wave2);

    return Level(cfg);
}