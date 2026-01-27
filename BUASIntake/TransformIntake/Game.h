#pragma once
#include <memory>
#include <vector>
#include "Level.h"
class Game
{
public:
	Game();
	bool init();
	void run();
	void quit() { running = false; }

private:
	void handleInput();
	void update(float dt);
	void render();

	bool running = false;
	std::vector<Level> levels;
	int currentLevelIndex = 0;
};

