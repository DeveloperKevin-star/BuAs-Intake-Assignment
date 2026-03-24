#pragma once
#include <vector>
#include <memory>
#include "Enemy.h"
#include "Projectile.h"

class Tower
{
public:
    Tower(float x, float y, float range, float fireRate, int damage);

    void update(float dt,
        std::vector<std::unique_ptr<Enemy>>& enemies,
        std::vector<std::unique_ptr<Projectile>>& projectiles,
        const sf::Texture& projectileTexture,
        const std::vector<sf::IntRect>& projectileRects);

    float getX() const { return x; }
    float getY() const { return y; }
    float getRange() const { return range; }
    float getFireFlashTimer() const { return fireFlashTimer; }
    float getFireFlashDuration() const { return fireFlashDuratio; }

private:
    float x = 0.0f;
    float y = 0.0f;
    float range = 100.0f;
    float fireRate = 1.0f;
    float fireCooldown = 0.0f;
    int damage = 1;
    float fireFlashTimer = 0.0f;
    float fireFlashDuratio = 0.08f;

    Enemy* findTarget(std::vector<std::unique_ptr<Enemy>>& enemies);
};