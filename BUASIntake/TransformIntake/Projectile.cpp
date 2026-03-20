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
    : x(startX), 
      y(startY), 
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

    float dx = target->getX() - x;
    float dy = target->getY() - y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < 4.0f)
    {
        hitTarget = true;
        return;
    }

    pos += vel * dt;

    sprite.setPosition(pos);

    //float moveDistance = speed * dt;

    //if (moveDistance >= distance)
    //{
    //    x = target->getX();
    //    y = target->getY();
    //    hitTarget = true;
    //}
    //else
    //{
    //    x += (dx / distance) * moveDistance;
    //    y += (dy / distance) * moveDistance;
    //}
}
void Projectile::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}