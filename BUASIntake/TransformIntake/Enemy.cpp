#include "Enemy.h"
#include <cmath>

Enemy::Enemy(EnemyType enemyType, const std::vector<PathNode>& enemyPath)
    : type(enemyType), path(enemyPath)
{
    applyTypeStats();

    if (!path.empty())
    {
        x = path[0].x;
        y = path[0].y;
        targetNodeIndex = path.size() > 1 ? 1 : 0;
    }
}

void Enemy::applyTypeStats()
{
    switch (type)
    {
    case EnemyType::Smog:
        health = 8;
        speed = 45.0f;
        pollutionDamage = 1;
        break;
    case EnemyType::Plastic:
        health = 15;
        speed = 30.0f;
        pollutionDamage = 2;
        break;
    case EnemyType::Oil:
        health = 25;
        speed = 20.0f;
        pollutionDamage = 4;
        break;
    }
}

void Enemy::update(float dt)
{
    if (reachedGoal || !isAlive() || path.size() < 2)
        return;

    if (targetNodeIndex >= path.size())
    {
        reachedGoal = true;
        return;
    }

    const PathNode& target = path[targetNodeIndex];

    float dx = target.x - x;
    float dy = target.y - y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < 0.001f)
    {
        ++targetNodeIndex;
        if (targetNodeIndex >= path.size())
            reachedGoal = true;
        return;
    }

    float moveDistance = speed * dt;

    if (moveDistance >= distance)
    {
        x = target.x;
        y = target.y;
        ++targetNodeIndex;

        if (targetNodeIndex >= path.size())
            reachedGoal = true;
    }
    else
    {
        x += (dx / distance) * moveDistance;
        y += (dy / distance) * moveDistance;
    }
}

void Enemy::takeDamage(int damage)
{
    health -= damage;
}