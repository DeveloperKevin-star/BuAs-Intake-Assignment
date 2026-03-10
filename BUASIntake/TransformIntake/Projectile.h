#pragma once
#include "Enemy.h"

class Projectile
{
public:
    Projectile(float startX, float startY, Enemy* target, float speed, int damage);

    void update(float dt);

    bool hasHitTarget() const { return hitTarget; }
    bool isExpired() const { return expired; }

    Enemy* getTarget() const { return target; }
    int getDamage() const { return damage; }

    float getX() const { return x; }
    float getY() const { return y; }

private:
    float x = 0.0f;
    float y = 0.0f;
    float speed = 200.0f;
    int damage = 1;

    Enemy* target = nullptr;

    bool hitTarget = false;
    bool expired = false;
};