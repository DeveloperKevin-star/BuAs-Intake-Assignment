#pragma once
#include <vector>
#include <memory>
#include "Enemy.h"

class Tower
{
public:
    Tower(float x, float y, float range, float fireRate, int damage);

    void update(float dt, std::vector<std::unique_ptr<Enemy>>& enemies);

    float getX() const { return x; }
    float getY() const { return y; }

private:
    float x = 0.0f;
    float y = 0.0f;
    float range = 100.0f;
    float fireRate = 1.0f;
    float fireCooldown = 0.0f;
    int damage = 1;

    Enemy* findTarget(std::vector<std::unique_ptr<Enemy>>& enemies);
};