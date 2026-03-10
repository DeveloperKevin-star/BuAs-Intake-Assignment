#include "Level.h"

Level::Level(const LevelConfig& cfg)
    : config(cfg), ecoSystemHealth(cfg.initialHealth)
{
    // Example starter tower
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

void Level::spawnEnemies(float dt)
{
    if (currentWaveIndex >= static_cast<int>(config.waves.size()))
        return;

    waveTimer += dt;
    WaveConfig& wave = config.waves[currentWaveIndex];

    while (!wave.enemiesToSpawn.empty() && waveTimer >= wave.spawnInterval)
    {
        waveTimer -= wave.spawnInterval;

        EnemyType type = wave.enemiesToSpawn.back();
        wave.enemiesToSpawn.pop_back();

        enemies.push_back(std::make_unique<Enemy>(type, config.enemyPath));

        if (wave.enemiesToSpawn.empty())
        {
            ++currentWaveIndex;
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
    // Not needed yet if towers directly damage enemies.
    // Use this later if you add projectile movement.
}

void Level::render()
{
    // Hook this up to SDL/SFML/raylib later.
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