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
    updateEffects(dt);

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

void Level::handleCollision()
{
    // Not needed yet, because hit handling happens in updateProjectiles()
}

void Level::render(sf::RenderWindow& window)
{
    drawPath(window);
    drawEnemies(window);
    drawEffects(window);
    drawTowers(window);
    drawProjectiles(window);
}

//Enemies Functions
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
            spawnDeathFlash((*it)->getX(), (*it)->getY());
            money += killReward;
            it = enemies.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

// Path Functios
void  Level::drawPath(sf::RenderWindow& window)
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
}

//Enemies functions
void  Level::drawEnemies(sf::RenderWindow& window) // in the function the enemies and their healthbars are drawn
{
    for (const auto& enemy : enemies)
    {
        //this draws the enemies
        sf::CircleShape shape(10.0f);
        shape.setOrigin(10.0f, 10.0f);
        shape.setPosition(enemy->getX(), enemy->getY());
        shape.setFillColor(getEnemyColor(enemy->getType()));

        window.draw(shape);

        drawEnemyHealthbars(window, *enemy);

    }
}

sf::Color Level::getEnemyColor(EnemyType type) const //this function is the collouring of the enemies, this make expansion easier
{
    //this switch is to make the enemies different from each other so its easier for the player to identifie them
    switch (type)
    {
    case EnemyType::Smog:
        return sf::Color(120, 120, 120); // smog is gray
        break;

    case EnemyType::Plastic:
        return sf::Color(255, 220, 50); // plastic is yellow
        break;

    case EnemyType::Oil:
        return sf::Color(40, 40, 40); // oil is dark gray/black
        break;

    default:
        return sf::Color::Magenta; // debugging
    }
}

sf::Color Level::getHealthBarColor(float healthPercentage) const
{
    if (healthPercentage > 0.6f)
    {
        return sf::Color::Green;
    }
    else if (healthPercentage > 0.3f)
    {
        return sf::Color::Yellow;
    }
    else
    {
        return sf::Color::Red;
    }
}

void Level::drawEnemyHealthbars(sf::RenderWindow& window, const Enemy& enemy)
{
    //These are the enemy health bar settings
    const float barWidth = 20.0f;
    const float barHeight = 4.0f;
    const float barOffsetY = 18.0f;

    float healthPercent = 0.0f;

    if (enemy.getMaxHealth() > 0) // this a safety measure so that game doesnt try to divide by zero
    {
        healthPercent = static_cast<float>(enemy.getHealth()) / enemy.getMaxHealth();
    }

    sf::RectangleShape backBar(sf::Vector2f(barWidth, barHeight));
    backBar.setFillColor(sf::Color(120, 0, 0));
    backBar.setOrigin(barWidth / 2.0f, barHeight / 2.0f);
    backBar.setPosition(enemy.getX(), enemy.getY() - barOffsetY);
    window.draw(backBar);

    sf::RectangleShape frontBar(sf::Vector2f(barWidth * healthPercent, barHeight));
    frontBar.setFillColor(sf::Color::Green);
    frontBar.setOrigin(barWidth / 2.0f, barHeight / 2.0f);
    frontBar.setPosition(enemy.getX(), enemy.getY() - barOffsetY);
    frontBar.move(-(barWidth * (1.0f - healthPercent)) / 2.0f, 0.0f);
    window.draw(frontBar);
}

void Level::spawnDeathFlash(float x, float y)
{
    FlashEffect flash;
    flash.x = x;
    flash.y = y;
    flash.timer = flash.duration;

    deathFlashes.push_back(flash);
}

void Level::updateEffects(float dt) 
{
    for (auto it = deathFlashes.begin(); it != deathFlashes.end();)
    {
        it->timer -= dt;

        if (it->timer <= 0.0f)
        {
            it = deathFlashes.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Level::drawEffects(sf::RenderWindow& window)
{
    for (const auto& flash : deathFlashes)
    {
        float percent = flash.timer / flash.duration;
        float radius = 10.f + (1.f - percent) * 12.f; 

        sf::CircleShape flashShape(radius);
        flashShape.setOrigin(radius, radius);
        flashShape.setPosition(flash.x, flash.y);
        flashShape.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(180 * percent)));

        window.draw(flashShape);
    }
}

// tower functions
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

void Level::drawTowers(sf::RenderWindow& window)
{
    //this draws the towers
    for (const auto& tower : towers)
    {
        sf::CircleShape shape(15.0f);
        shape.setFillColor(sf::Color::Green);
        shape.setOrigin(15.0f, 15.0f);
        shape.setPosition(tower->getX(), tower->getY());
        window.draw(shape);

        //This is the fire flash drawing system
        if (tower->getFireFlashTimer() > 0.0f)
        {
            float percent = tower->getFireFlashTimer() / tower->getFireFlashDuration();

            sf::CircleShape flash(20.f);
            flash.setOrigin(20.f, 20.f);
            flash.setPosition(tower->getX(), tower->getY());
            flash.setFillColor(sf::Color(255, 255, 100, static_cast<sf::Uint8>(150 * percent)));

            window.draw(flash); 
        }
    }
}

void Level::drawProjectiles(sf::RenderWindow& window)
{
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

//Levels + Waves
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

bool Level::hasMoreWaves() const
{
    return currentWaveIndex < static_cast<int>(config.waves.size());
}

float Level::getNextSpawnCountdown() const
{
    if (!hasMoreWaves())
        return 0.f;

    const WaveConfig& wave = config.waves.at(currentWaveIndex);
    if (wave.enemiesToSpawn.empty())
        return 0.f;

    float remaining = wave.spawnInterval - waveTimer;

    if (remaining < 0.f)
        remaining = 0.f;

    return remaining;
}