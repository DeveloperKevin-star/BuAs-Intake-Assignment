#include "Level.h"

Level::Level(const LevelConfig& cfg)
	: config(cfg), EcoSystemHealth(cfg.initialHealth) {
}

void Level::update(float dt) {
	if (completed || failed) return;

	spawnEnemies(dt);
	updateEnemies(dt);
	updateTowers(dt);
	handleCollision();


	// this handles the level health, so that you fail when you die "eco system" reaches zero of that if you completed all the waves with all the enemies killed that 
	// the level is completed
	if (EcoSystemHealth <= 0) {
		ffailed = true;
	}
	else if (currentWaveIndex >= (int)config.waves.size()
		&& enemies.empty()) {
		completed = true;
	}
}
// this function handles the enemy spawning with sizes of each wave. It also takes care that wave number goes up when a wave is done
void Level::spawnEnemies(float dt) {
	if (currentWaveIndex >= (int)config.waves.size()) return;

	waveTimer += dt;
	WaveConfig& wave = config.waves[currentWaveIndex];

	while (!wave.enemiesToSpawn.empty() &&
		waveTimer >= wave.spawnInterval) {
		waveTimer -= wave.spawnInterval;

		EnemyType type = wave.enemiesToSpawn.back();
		wave.enemiesToSpawn.pop_back();

		enemies.push_back(std::make_unique<Enemy>(type, config.enemyPath));

		if (wave.enemiesToSpawn.empty()) {
			++currentWaveIndex;
		}
	}
}


// This function updates the enmeies. so that they will erase when they die/reach the end and the player loses health once an enemmy reaches the end
void Level::updateEnemies(float dt) {
	for (auto it = enemies.begin(); it != enemies.end()); {
		(*it)->update(dt);
		if ((*it))->hasReachedGoal()){
			EcoSystemHealth -= (*it)->getPollutionDamage();
			it = enemies.erase(it);
		}
		else if (!(*it)->isAlive()) {
			it = enemies.erase(it)
		}
		else {
			++it;
		}
	}
}

void Level::updateTowers(float dt) {
	for (auto& tower : towers) {
		tower->update(dt, enemies00);
	}
}

Level Level::createCitySmogLevel() {
	LevelCondig cfg;
	cfg.name = "City Smog Defence";
	cfg.initialHealth = 50;

	return Level(cfg);
}