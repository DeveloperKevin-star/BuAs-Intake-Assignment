#include "Projectile.h"
#include "SFML/Graphics.hpp"
#include <cmath>

Projectile::Projectile(
    const sf::Texture& texture,
    float startX, 
    float startY,
    Enemy* targetEnemy, 
    float projectileSpeed, 
    int projectileDamage
)
    : pos(startX,startY), 
      speed(projectileSpeed), 
      damage(projectileDamage), 
      target(targetEnemy)
{
    //-- This sets the size of the sprite --
    sprite.setTexture(texture, true);
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    
    float desiredSize = 10.f;
    float scale = desiredSize / bounds.width;
    sprite.setScale(scale, scale);

    sprite.setPosition(pos);
}

void Projectile::update(float dt)//--- This updates the projectile when needed ---
{
    if (expired || hitTarget)
        return;

    if (!target || !target->isAlive() || target->hasReachedGoal()) //-- This checks if a projectile has hit their target --
    {
        expired = true;
        return;
    }

    sf::Vector2f toTarget(target->getX() - pos.x, target->getY() - pos.y);//-- This handles the distance between a projectile and their target --
    float distance = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);

    if (distance < 10.0f)
    {
        hitTarget = true;
        return;
    }

    if (distance > .0f) //-- This handles the movespeed of the projectile --
    {
        sf::Vector2f direction = toTarget / distance;
        float moveDistance = speed * dt;

        if (moveDistance >= distance)
        {
            pos.x = target->getX();
            pos.y = target->getY();
            sprite.setPosition(pos);
            hitTarget = true;
            return;
        }

        pos += direction * moveDistance;
        sprite.setPosition(pos);

        float angle = std::atan2(direction.y, direction.x) * 90.f / 3.14159265f;
        sprite.setRotation(angle);
    } 
}
void Projectile::render(sf::RenderWindow& window)//--- This draws the projectile ---
{
    window.draw(sprite);
}