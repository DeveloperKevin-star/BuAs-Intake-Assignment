#include "Projectile.h"
#include <cmath>

Projectile::Projectile(float startX, float startY, Enemy* targetEnemy, float projectileSpeed, int projectileDamage)
    : x(startX), y(startY), speed(projectileSpeed), damage(projectileDamage), target(targetEnemy)
{
}

void Projectile::update(float dt)
{
    if (expired || hitTarget)
        return;

    if (!target || !target->isAlive() || target->hasReachedGoal())
    {
        expired = true;
        return;
    }

    float dx = target->getX() - x;
    float dy = target->getY() - y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < 4.0f)
    {
        hitTarget = true;
        return;
    }

    float moveDistance = speed * dt;

    if (moveDistance >= distance)
    {
        x = target->getX();
        y = target->getY();
        hitTarget = true;
    }
    else
    {
        x += (dx / distance) * moveDistance;
        y += (dy / distance) * moveDistance;
    }
}