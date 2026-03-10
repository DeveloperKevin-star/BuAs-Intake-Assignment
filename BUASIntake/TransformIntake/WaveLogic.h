#pragma once
#include <vector>
#include <string>
#include "PathNode.h"

//Here are all the enemy types in the game
enum class EnemyType
{
    Smog,
    Plastic,
    Oil
};

//Here it says which enemies are in a wave
struct WaveConfig
{
    std::vector<EnemyType> enemiesToSpawn;
    float spawnInterval = 1.0f;
};

//This states what a level contains
struct LevelConfig
{
    std::string name;
    int initialHealth = 50;
    std::vector<PathNode> enemyPath;
    std::vector<WaveConfig> waves;
};