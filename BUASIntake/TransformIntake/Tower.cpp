#include "Tower.h"
#include <cmath>
#include <limits>

Tower::Tower(float px, float py, float towerRange, float towerFireRate, int towerDamage)
    : x(px), y(py), range(towerRange), fireRate(towerFireRate), damage(towerDamage)
{
}

Enemy* Tower::findTarget(std::vector<std::unique_ptr<Enemy>>& enemies)
{
    Enemy* bestTarget = nullptr;
    float bestDistance = std::numeric_limits<float>::max();

    for (auto& enemy : enemies)
    {
        if (!enemy || !enemy->isAlive() || enemy->hasReachedGoal())
            continue;

        float dx = enemy->getX() - x;
        float dy = enemy->getY() - y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist <= range && dist < bestDistance)
        {
            bestDistance = dist;
            bestTarget = enemy.get();
        }
    }

    return bestTarget;
}

void Tower::update(float dt,
    std::vector<std::unique_ptr<Enemy>>& enemies,
    std::vector<std::unique_ptr<Projectile>>& projectiles,
    const sf::Texture& projectileTexture,
    const std::vector<sf::IntRect>& projectileRects)
{
    
    // This is the fire flash system
    if (fireFlashTimer > 0.0f)
    {
        fireFlashTimer -= dt;
        if (fireFlashTimer < 0.0f)
            fireFlashTimer = 0.0f;
    }
    
    // This is fire rate code
    if (fireRate <= 0.0f)
        return;

    fireCooldown -= dt;

    if (fireCooldown > 0.0f)
        return;

    // this is the enemy target system
    Enemy* target = findTarget(enemies);
    if (target)
    {
        projectiles.push_back(std::make_unique<Projectile>(
            projectileTexture, 
            projectileRects,
            x, 
            y, 
            target->getX(),
            target->getY(),
            target,
            400.0f, damage));
        fireCooldown = 1.0f / fireRate;
        fireFlashTimer = fireFlashDuratio;
    }
}