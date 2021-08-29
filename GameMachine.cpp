#include "GameMachine.h"
#include <iostream>
#include <algorithm>
#include <ctime>

bool GameMachine::isFinished()
{
	for (int i = 1; i < 4; i++) {
		for (int j = 1; j < 4; j++) {
			if (gameState[i][j].value == 0) {
				return false;
			}
			if (gameState[i][j].value == 2048) {
				bIsSuccess = true;
				return true;
			}
			if (gameState[i][j].value == gameState[i - 1][j].value || gameState[i][j].value == gameState[i][j - 1].value) {
				return false;
			}
		}
	}

	for (int i = 1; i < 4; i++) {
		if (gameState[0][i].value == 0) {
			return false;
		}
		if (gameState[0][i].value == 2048) {
			bIsSuccess = true;
			return true;
		}
		if (gameState[0][i].value == gameState[0][i - 1].value) {
			return false;
		}
	}

	for (int i = 1; i < 4; i++) {
		if (gameState[i][0].value == 0) {
			return false;
		}
		if (gameState[i][0].value == 2048) {
			bIsSuccess = true;
			return true;
		}
		if (gameState[i][0].value == gameState[i - 1][0].value) {
			return false;
		}
	}

	return true;
}

void GameMachine::updateEmptySequence()
{
	emptySequence.clear();
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			emptySequence.push_back(glm::ivec3(i, j, gameState[i][j].value == 0 ? 0 : 1));
		}
	}
	srand((unsigned)time(NULL));
	std::random_shuffle(emptySequence.begin(), emptySequence.end());
}

GameMachine::GameMachine()
{
	Point point;
	point.value = 0;
	gameState = std::vector<std::vector<Point>>(4, std::vector<Point>(4, point));

	//add all of the empty position into vector
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			emptySequence.push_back(glm::ivec3(i, j, 0));
		}
	}
	spawnNewNumber();

	score = 0;
	bIsSuccess = false;
}

GameMachine::GameMachine(const GameMachine& c)
{
	this->bIsSuccess = c.bIsSuccess;
	this->gameState = c.gameState;
	this->emptySequence = c.emptySequence;
	this->score = c.score;
}

std::vector<std::vector<Point>> GameMachine::getGameState(bool& bIsFinished, bool& bIsSuccess)
{
	bIsSuccess = this->bIsSuccess;
	bIsFinished = isFinished();
	return gameState;
}

unsigned int GameMachine::getScore()
{
	return score;
}

void GameMachine::pressRight()
{
	//let the number close to right
	for (int i = 0; i < 4; i++) {
		std::vector<Point> newRow;
		for (int j = 0; j < 4; j++) {
			if (gameState[i][j].value != 0) {
				newRow.push_back(gameState[i][j]);
			}
		}
		for (int j = newRow.size(); j < 4; j++) {
			Point newPoint;
			newPoint.value = 0;
			newRow.insert(newRow.begin(), newPoint);
		}
		gameState[i] = newRow;
	}

	//calculate the result
	for (int i = 0; i < 4; i++) {
		for (int j = 3; j > 0; j--) {
			if (gameState[i][j].value == gameState[i][j - 1].value) {
				score += gameState[i][j].value;
				gameState[i][j].value *= 2;
				gameState[i][j - 1].value = 0;
			}
		}
	}

	//repeat
	for (int i = 0; i < 4; i++) {
		std::vector<Point> newRow;
		for (int j = 0; j < 4; j++) {
			if (gameState[i][j].value != 0) {
				newRow.push_back(gameState[i][j]);
			}
		}
		for (int j = newRow.size(); j < 4; j++) {
			Point newPoint;
			newPoint.value = 0;
			newRow.insert(newRow.begin(), newPoint);
		}
		gameState[i] = newRow;
	}

	updateEmptySequence();
	spawnNewNumber();
}

void GameMachine::pressUp()
{
	//let the number close to up
	for (int i = 0; i < 4; i++) {
		std::vector<Point> newColumn;
		int count = 0;
		for (int j = 0; j < 4; j++) {
			if (gameState[j][i].value != 0) {
				newColumn.push_back(gameState[j][i]);
				count++;
			}
		}
		for (int j = 0; j < count; j++) {
			gameState[j][i] = newColumn[j];
		}
		for (int j = count; j < 4; j++) {
			Point newPoint;
			newPoint.value = 0;
			gameState[j][i] = newPoint;
		}
	}


	//calculate the result
	for (int i = 1; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (gameState[i][j].value == gameState[i - 1][j].value) {
				score += gameState[i][j].value;
				gameState[i - 1][j].value *= 2;
				gameState[i][j].value = 0;
			}
		}
	}

	//let the number close to up
	for (int i = 0; i < 4; i++) {
		std::vector<Point> newColumn;
		int count = 0;
		for (int j = 0; j < 4; j++) {
			if (gameState[j][i].value != 0) {
				newColumn.push_back(gameState[j][i]);
				count++;
			}
		}
		for (int j = 0; j < count; j++) {
			gameState[j][i] = newColumn[j];
		}
		for (int j = count; j < 4; j++) {
			Point newPoint;
			newPoint.value = 0;
			gameState[j][i] = newPoint;
		}
	}

	updateEmptySequence();
	spawnNewNumber();
}

void GameMachine::pressDown()
{
	//let the number close to down
	for (int i = 0; i < 4; i++) {
		std::vector<Point> newColumn;
		for (int j = 0; j < 4; j++) {
			if (gameState[j][i].value != 0) {
				newColumn.push_back(gameState[j][i]);
			}
		}

		for (int j = 0; j < 4 - newColumn.size(); j++) {
			Point newPoint;
			newPoint.value = 0;
			gameState[j][i] = newPoint;
		}
		for (int j = 4 - newColumn.size(), k = 0; j < 4; j++, k++) {
			gameState[j][i] = newColumn[k];
		}
	}

	//calculate the result
	for (int i = 3; i > 0; i--) {
		for (int j = 0; j < 4; j++) {
			if (gameState[i][j].value == gameState[i - 1][j].value) {
				score += gameState[i][j].value;
				gameState[i][j].value *= 2;
				gameState[i - 1][j].value = 0;
			}
		}
	}

	//let the number close to down
	for (int i = 0; i < 4; i++) {
		std::vector<Point> newColumn;
		for (int j = 0; j < 4; j++) {
			if (gameState[j][i].value != 0) {
				newColumn.push_back(gameState[j][i]);
			}
		}

		for (int j = 0; j < 4 - newColumn.size(); j++) {
			Point newPoint;
			newPoint.value = 0;
			gameState[j][i] = newPoint;
		}
		for (int j = 4 - newColumn.size(), k = 0; j < 4; j++, k++) {
			gameState[j][i] = newColumn[k];
		}
	}

	updateEmptySequence();
	spawnNewNumber();
}

void GameMachine::pressLeft()
{
	//let the number close to left
	for (int i = 0; i < 4; i++) {
		std::vector<Point> newRow;
		for (int j = 0; j < 4; j++) {
			if (gameState[i][j].value != 0) {
				newRow.push_back(gameState[i][j]);
			}
		}
		for (int k = newRow.size(); k < 4; k++) {
			Point newPoint;
			newPoint.value = 0;
			newRow.push_back(newPoint);
		}
		gameState[i] = newRow;
	}

	//calculate the result
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j < 4; j++) {
			if (gameState[i][j].value == gameState[i][j - 1].value) {
				score += gameState[i][j].value;
				gameState[i][j - 1].value *= 2;
				gameState[i][j].value = 0;
			}
		}
	}

	//repeat
	for (int i = 0; i < 4; i++) {
		std::vector<Point> newRow;
		for (int j = 0; j < 4; j++) {
			if (gameState[i][j].value != 0) {
				newRow.push_back(gameState[i][j]);
			}
		}
		for (int k = newRow.size(); k < 4; k++) {
			Point newPoint;
			newPoint.value = 0;
			newRow.push_back(newPoint);
		}
		gameState[i] = newRow;
	}

	updateEmptySequence();
	spawnNewNumber();
}

void GameMachine::spawnNewNumber()
{
	srand((unsigned)time(NULL));
	int ran = rand() % 10;
	std::random_shuffle(emptySequence.begin(), emptySequence.end());
	for (int i = 0; i < emptySequence.size(); i++) {
		if (emptySequence[i].z == 0) {
			emptySequence[i].z = 1;
			int x = emptySequence[i].x;
			int y = emptySequence[i].y;
			if (ran <= 1) {
				gameState[x][y].value = 4;
			}
			else {
				gameState[x][y].value = 2;
			}
			
			break;
		}
	}
}

void GameMachine::printGameState()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << gameState[i][j].value << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}