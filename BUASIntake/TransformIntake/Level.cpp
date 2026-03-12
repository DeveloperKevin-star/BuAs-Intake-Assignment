#include "Level.h"
#include <cmath>

Level::Level(const LevelConfig& cfg)
    : config(cfg), ecoSystemHealth(cfg.initialHealth)
{
    towers.push_back(std::make_unique<Tower>(150.0f, 120.0f, 100.0f, 1.0f, 3));
}

void Level::update(float dt)
{
    if (completed || failed)
        return;

    spawnEnemies(dt);
    updateEnemies(dt);
    updateTowers(dt);
    updateProjectiles(dt);
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

void Level::spawnEnemies(float dt)
{
    if (currentWaveIndex >= static_cast<int>(config.waves.size()))
        return;

    waveTimer += dt;
    //WaveConfig& wave = config.waves[currentWaveIndex];
    WaveConfig& wave = config.waves.at(currentWaveIndex);

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
        tower->update(dt, enemies, projectiles);
    }
}

void Level::updateProjectiles(float dt)
{
    for (auto it = projectiles.begin(); it != projectiles.end();)
    {
        (*it)->update(dt);

        if ((*it)->hasHitTarget())
        {
            Enemy* target = (*it)->getTarget();
            if (target && target->isAlive() && !target->hasReachedGoal())
            {
                target->takeDamage((*it)->getDamage());
            }

            it = projectiles.erase(it);
        }
        else if ((*it)->isExpired())
        {
            it = projectiles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Level::handleCollision()
{
    // Not needed yet, because hit handling happens in updateProjectiles()
}

void Level::render(sf::RenderWindow& window)
{
    //this draws the path
    for (size_t i = 1; i < config.enemyPath.size(); ++i)
    {
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(config.enemyPath[i - 1].x, config.enemyPath[i - 1].y), sf::Color::White),
            sf::Vertex(sf::Vector2f(config.enemyPath[i].x, config.enemyPath[i].y), sf::Color::White)
        };

        window.draw(line, 2, sf::Lines);
    }


    //this draws the enemies
    for(const auto& enemy : enemies)
    {
        sf::CircleShape shape(10.0f);
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin(10.0f, 10.0f);
        shape.setPosition(enemy->getX(), enemy->getY());
        window.draw(shape);
    }

    //this draws the towers
    for (const auto& tower : towers)
    {
        sf::CircleShape shape(15.0f);
        shape.setFillColor(sf::Color::Green);
        shape.setOrigin(15.0f, 15.0f);
        shape.setPosition(tower->getX(), tower->getY());
        window.draw(shape);
    }

    // this is drawing the projectiles
    for (const auto& projectile : projectiles)
    {
        sf::CircleShape shape(4.0f);
        shape.setFillColor(sf::Color::Yellow);
        shape.setOrigin(4.0f, 4.0f);
        shape.setPosition(projectile->getX(), projectile->getY());
        window.draw(shape);
    }
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