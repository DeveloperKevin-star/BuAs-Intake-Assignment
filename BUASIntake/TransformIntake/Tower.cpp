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

void Tower::update(float dt, std::vector<std::unique_ptr<Enemy>>& enemies)
{
    fireCooldown -= dt;

    if (fireCooldown > 0.0f)
        return;

    Enemy* target = findTarget(enemies);
    if (target)
    {
        target->takeDamage(damage);
        fireCooldown = 1.0f / fireRate;
    }
}