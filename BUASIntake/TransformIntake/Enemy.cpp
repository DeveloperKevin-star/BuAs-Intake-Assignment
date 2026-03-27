#include "Enemy.h"
#include "SFML/Graphics.hpp"
#include <cmath>

Enemy::Enemy(EnemyType enemyType, const std::vector<PathNode>& enemyPath)
    : type(enemyType), path(enemyPath)
{
    applyTypeStats();

    //-- This checks if there is a path to follow and then will follow that path --
    if (!path.empty())
    {
        x = path.at(0).x;
        y = path.at(0).y;
        targetNodeIndex = (path.size() > 1) ? 1 : 0;
    }
}

void Enemy::applyTypeStats()    //--- This Functions applies the stats to different enemy types ---
{

    switch (type)
    {
    case EnemyType::Smog:
        health = 8;
        maxHealth = 8;
        speed = 45.0f;
        pollutionDamage = 1;
        killReward = 5;
        break;

    case EnemyType::Plastic:
        health = 15;
        maxHealth = 15;
        speed = 30.0f;
        pollutionDamage = 2;
        killReward = 10;
        break;

    case EnemyType::Oil:
        health = 25;
        maxHealth = 25;
        speed = 20.0f;
        pollutionDamage = 4;
        killReward = 20;
        break;
    }
}

void Enemy::update(float dt) //--- This checks if an enemy has reached the end to see if the enemy still needs to move ---
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

        const PathNode& target = path.at(targetNodeIndex);

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

void Enemy::takeDamage(int damage)
{
    health -= damage;
}

const sf::Sprite& Enemy::getSprite() const
{
    return sprite;
}

const sf::Texture& Enemy::getTexture() const
{
    return texture;
}