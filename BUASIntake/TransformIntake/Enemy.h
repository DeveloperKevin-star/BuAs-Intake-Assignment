#pragma once
#include <vector>
#include "PathNode.h"
#include "WaveLogic.h"

//This class contains the enemy base logic, with the type of enemy, what their path is, their current position,
//their movement speed, health, damage to the player if reaches the end, the next node it is moving to and if it reached the end

class Enemy
{
public:
    Enemy(EnemyType type, const std::vector<PathNode>& path);

    void update(float dt);
    void takeDamage(int damage);

    bool isAlive() const { return health > 0; }
    bool hasReachedGoal() const { return reachedGoal; }
    int getPollutionDamage() const { return pollutionDamage; }

    float getX() const { return x; }
    float getY() const { return y; }

    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    EnemyType getType() const {return type;}

private:
    EnemyType type;
    std::vector<PathNode> path;

    float x = 0.0f;
    float y = 0.0f;
    float speed = 40.0f;

    int health = 10;
    int maxHealth = 10;
    int pollutionDamage = 1;

    size_t targetNodeIndex = 1;
    bool reachedGoal = false;

    void applyTypeStats();
};