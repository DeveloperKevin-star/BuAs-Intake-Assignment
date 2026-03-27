#pragma once
#include <vector>
#include <memory>
#include "SFML/Graphics.hpp"
#include "Enemy.h"
#include "Tower.h"
#include "Projectile.h"
#include "PathNode.h"
#include "WaveLogic.h"

struct InputState
{
    float mouseX = 0.f;
    float mouseY = 0.f;
    bool placeTowerPressed = false;
};

struct FlashEffect 
{
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f baseScale{ 1.f, 1.f };
    float timer = .0f;
    float duration = .35f;
};

class Level
{
public:
    //game
    Level(const LevelConfig& config);

    void addMoney(int amount) { money += amount; }

    void update(float dt);
    void render(sf::RenderWindow& window);

    void handlePlayerInput(const InputState& input);

    //tower
    bool placeTowerAt(float x, float y);
    bool canPlaceTowerAt(float x, float y) const;
    float getPlacementTowerRange() const { return placementTowerRange; }

    //Level
    bool isCompleted() const { return completed; }
    bool isFailed() const { return failed; }
    float getCleunupProgress() const;


    int getEcoSystemHealth() const { return ecoSystemHealth; }
    int getMoney() const { return money; }
    int getCurrentWaveIndex() const { return currentWaveIndex; }
    int getTowerCost() const { return towerCost; }
    int getBackgroundStage() const;
    const std::string& getName() const { return config.name; }

    static Level createCitySmogLevel();
    static Level createIndustrialLevel();
    static Level createHarborLevel();
    static Level createToxicLevel();
    static Level createMFactoryLevel();



    //waves
    float getNextSpawnCountdown() const;
    bool hasMoreWaves() const;

private:
    //--- Enemy functions ---
    void spawnEnemies(float dt);
    void updateEnemies(float dt);
    void drawEnemies(sf::RenderWindow& window);
    sf::Color getEnemyColor(EnemyType type) const; // this handles the enemy colors
    sf::Color getHealthBarColor(float healthPercent) const;
    void drawEnemyHealthbars(sf::RenderWindow& window, const Enemy& enemy);
    int enemiesKilled = 0;
    int totalEnemies = 0;
    //Flashes
    std::vector<FlashEffect> deathFlashes; // this handles the deathflashes of the enemies
    void updateEffects(float dt);
    void drawEffects(sf::RenderWindow& window);
    void spawnDeathFlash(const sf::Sprite& enemySprite, const sf::Texture& enemyTexture);

    //--- Textures ---
    sf::Texture smogTexture;
    sf::Texture plasticTexture;
    sf::Texture oilTexture;
    sf::Texture towerTexture;
    sf::Texture projectileTexture;

    std::vector<sf::Texture> backgroundTextures;
    std::vector<sf::Sprite> backgroundSprites;

    sf::Texture pathTexture;
   
    bool loadTextures();
    bool loadLevelVisual();

    const sf::Texture& getEnemyTexture(EnemyType type) const;
    void drawBackground(sf::RenderWindow& window);


    //--- Tower functions ---
    void updateTowers(float dt);
    void updateProjectiles(float dt);
    void handleCollision();
    void drawTowers(sf::RenderWindow& window);
    void drawProjectiles(sf::RenderWindow& window);
    std::vector<sf::IntRect> projectileRects;

    //--- Path functions ---
    bool isTooCloseToPath(float x, float y) const;
    bool isTooCloseToTower(float x, float y) const;
    void drawPath(sf::RenderWindow& windwow);
    sf::Color getPathColor() const;


    //--- Config ---
    LevelConfig config;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<std::unique_ptr<Projectile>> projectiles;
    
    //--- Waves ---
    float waveTimer = .0f;
    int currentWaveIndex = 0;
    int ecoSystemHealth = 0;

    //money
    int money = 100;
    int towerCost = 50;
    int killReward = 10;

    //level status
    bool completed = false;
    bool failed = false;

    float placementTowerRange = 100.f;
};