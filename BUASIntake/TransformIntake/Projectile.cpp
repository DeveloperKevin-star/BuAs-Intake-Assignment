#include "Projectile.h"
#include "SFML/Graphics.hpp"
#include <cmath>

Projectile::Projectile(
    const sf::Texture& texture,
    const std::vector<sf::IntRect>& spriteRects,
    float startX, 
    float startY,
    float targetX,
    float targetY,
    Enemy* targetEnemy, 
    float projectileSpeed, 
    int projectileDamage
)
    : pos(startX,startY), 
      speed(projectileSpeed), 
      damage(projectileDamage), 
      target(targetEnemy)
{
    sprite.setTexture(texture);

    //this function picks a random bullet from a sprite sheet
    int index = rand() % spriteRects.size();
    sprite.setTextureRect(spriteRects[index]);

    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    sprite.setScale(.12f, .12f);
    sprite.setPosition(pos);
     
    //Here the direction will be calculated
    sf::Vector2f dir(targetX - startX, targetY - startY);
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (length != 0)
    {
        dir /= length;
    }
   
    vel = dir * speed;
    
    //This rotates the sprite towards the direction
    float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
    sprite.setRotation(angle);
}

void Projectile::update(float dt)
{
    if (expired || hitTarget)
        return;

    if (!target || !target->isAlive() || target->hasReachedGoal())
    {
        expired = true;
        return;
    }

    sf::Vector2f toTarget(target->getX() - pos.x, target->getY() - pos.y);
    float distance = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);

    if (distance < 4.0f)
    {
        hitTarget = true;
        return;
    }

    if (distance > .0f) 
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

        float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
        sprite.setRotation(angle);
    } 
}
void Projectile::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}