#pragma once
#include "Enemy.h"
#include "SFML/Graphics.hpp"

class Projectile
{
public:
    Projectile(
        const sf::Texture& texture,
        const std::vector<sf::IntRect>& spriteRects,
        float startX,
        float startY,
        float targetX,
        float targetY,
        Enemy* target, 
        float speed, 
        int damage
    );

    void update(float dt);
    void render(sf::RenderWindow& window);

    bool hasHitTarget() const { return hitTarget; }
    bool isExpired() const { return expired; }

    Enemy* getTarget() const { return target; }
    int getDamage() const { return damage; }

    float getX() const { return pos.x; }
    float getY() const { return pos.y; }

private:
    sf::Vector2f pos;
    sf::Vector2f vel;
    
    sf::Sprite sprite;

    bool alive = true;


    float x = 0.0f;
    float y = 0.0f;
    float speed = 200.0f;
    int damage = 1;

    Enemy* target = nullptr;

    bool hitTarget = false;
    bool expired = false;
};