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
//This function applies the stats to each enemy type
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


//This checks whether an enemy has reached the goal, if they are still alive so can move the a new node if needed.
// also checks their movespeed
void Enemy::update(float dt)
{
    if (reachedGoal || !isAlive() || path.size() < 2)
        return;

    float remainingMove = speed * dt;

    while (remainingMove > 0.0f && !reachedGoal)
    {
        if (targetNodeIndex >= path.size())
        {
            reachedGoal = true;
            break;
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
            continue;
        }

        if (remainingMove >= distance)
        {
            x = target.x;
            y = target.y;
            remainingMove -= distance;
            ++targetNodeIndex;

            if (targetNodeIndex >= path.size())
                reachedGoal = true;
        }
        else
        {
            x += (dx / distance) * remainingMove;
            y += (dy / distance) * remainingMove;
            remainingMove = 0.0f;
        }
    }
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


//This handles the enemies health, and if returns certain points so that the game knows what an enemy has done. 
void Enemy::takeDamage(int damage)
{
    health -= damage;
}

bool Enemy::isAlive() const
{
    return health > 0;
}

bool Enemy::hasReachedGoal() const
{
    return reachedGoal;
}

int Enemy::getPollutionDamage() const
{
    return pollutionDamage;
}

float Enemy::getX() const
{
    return x;
}

float Enemy::getY() const
{
    return y;
}