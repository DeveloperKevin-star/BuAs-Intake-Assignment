#pragma once
#include <vector>
#include <memory>
#include "Enemy.h"
#include "Tower.h"
#include "PathNode.h"
#include "WaveLogic.h"

class Level
{
public:
	Level(const LevelConfig& config);
	void update(float dt);
	void render();

	bool isCompleted() const { return completed; }
	bool isFailed() const { return failed; }

	static Level createCitySmogLevel();

private:
	void SpawnEnemies(float dt);
	void UpdateEnemies(float dt);
	void UpdateTowers(float dt);
	void handleCollision();


	LevelConfig config;
	std::vector<std::unique_ptr<Enemy>> enemies;
	std::vector<std::unique_ptr<Tower>> towers;

	float waveTimer = 0.0f;
	int currentWaveIndex = 0;
	int EcoSystemHealth;
	bool completed = false;
	bool failed = false;
};

