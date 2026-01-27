#include "Game.h"

Game::Game() {}

bool Game::init() {
	// Here the levels, assets etc will be loaded into the game

	levels.emplace_back(Level::createCitySmogLevel());
	running = true;
	return true;
}


void Game::run() {

//This will handle the running of the game
	const float fixedDt = 1.0f / 60.0f;
	while (running) {
		handleInput();
		update(fixedDt);
		render();
	}
}

void Game::handleInput() {
	//This will handle the input of the game
}

void Game::update(float dt){
	if (currentLevelIndex < 0 || currentLevelIndex >= (int)Levels.size()) return;
	Level& lvl = levels[currentLevelIndex];

	// this handles the level quiting if there are no more levels or if the game is failed
	lvl.update(dt);
	if (lvl.isCompleted()) {
		++currentLevelIndex;
		if (currentLevelIndex >= (int)Levels.size()) {
			quit();
		}
	}
}

void Game::render() {
	//clear screen
	//levels[currentlevelindex]
	//present
}