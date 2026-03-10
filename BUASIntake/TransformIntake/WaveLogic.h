#pragma once
#include <vector>
#include <string>
#include "PathNode.h"

enum class EnemyType
{
    Smog,
    Plastic,
    Oil
};

struct WaveConfig
{
    std::vector<EnemyType> enemiesToSpawn;
    float spawnInterval = 1.0f;
};

struct LevelConfig
{
    std::string name;
    int initialHealth = 50;
    std::vector<PathNode> enemyPath;
    std::vector<WaveConfig> waves;
};