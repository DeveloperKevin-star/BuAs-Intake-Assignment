#pragma once
#include <vector>
#include "PathNode.h"
#include "WaveLogic.h"

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

private:
    EnemyType type;
    std::vector<PathNode> path;

    float x = 0.0f;
    float y = 0.0f;
    float speed = 40.0f;

    int health = 10;
    int pollutionDamage = 1;

    size_t targetNodeIndex = 1;
    bool reachedGoal = false;

    void applyTypeStats();
};