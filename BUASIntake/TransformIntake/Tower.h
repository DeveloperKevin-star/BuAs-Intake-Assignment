#pragma once
#include <vector>
#include <memory>
#include "Enemy.h"
#include "Projectile.h" 

//THis handles the stats for a tower
class Tower
{
public:
    Tower(float x, float y, float range, float fireRate, int damage);

    void update(float dt, std::vector<std::unique_ptr<Enemy>>& enemies);

    float getX() const { return x; }
    float getY() const { return y; }
    float getRange() const { return range; }

private:
    float x = 0.0f;
    float y = 0.0f;
    float range = 100.0f;
    float fireRate = 1.0f;
    float fireCooldown = 0.0f; // This created so that a tower does not fire every frame
    int damage = 1;

    Enemy* findTarget(std::vector<std::unique_ptr<Enemy>>& enemies);
};