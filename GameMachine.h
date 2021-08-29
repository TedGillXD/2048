#pragma once
#include "glm/glm.hpp"
#include <vector>

#include "Shader.h"
#include "Texture.h"

struct Point {
	unsigned int value;
};

class GameMachine
{
private:
	std::vector<std::vector<Point>> gameState;

	//in ivec3, x means position x, y means position y, z only contains 0 or 1, means empty when z=0
	std::vector<glm::ivec3> emptySequence;

	unsigned int score;

	bool bIsSuccess;

	bool isFinished();

	void updateEmptySequence();

	void spawnNewNumber();

public:
	GameMachine();

	GameMachine(const GameMachine& c);

	std::vector<std::vector<Point>> getGameState(bool& bIsFinished, bool& bIsSuccess);

	unsigned int getScore();

	void pressRight();

	void pressUp();

	void pressDown();

	void pressLeft();

	void printGameState();
};

