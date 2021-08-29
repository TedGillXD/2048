#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "GlobalVariable.h"
#include "Utils/ResourcesManager.h"
#include "GameMachine.h"
#include "Utils/Log.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//because the member function could not be a call_back function, the GameMachine instance must be declared to a global variable
GameMachine game;

#pragma region Function Declare
void processInput(GLFWwindow* window, int key, int scancode, int action, int mods);

void loadNumbers();

void renderBackground(Shader* shader, unsigned int VAO);

void renderNumber(Shader* shader, std::vector<std::vector<Point>> gameState, unsigned int VAO, unsigned int VBO);

void renderScore(Shader* shader, unsigned int VAO, unsigned int VBO);

void renderBest(Shader* shader, unsigned int VAO, unsigned int VBO);

void renderText(Shader* shader, std::string text, glm::vec3 textColor, unsigned int VAO, unsigned int VBO, float x, float y, float scale);
#pragma endregion

int main() {

	#pragma region Open Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window) {
		glfwTerminate();
		Log::e("create window fail");
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		Log::e("glew init fail");
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwSetKeyCallback(window, processInput);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	#pragma endregion

	#pragma region Init VAO, VBO
	unsigned int backgroundVAO, backgroundVBO;
	glGenVertexArrays(1, &backgroundVAO);
	glBindVertexArray(backgroundVAO);

	glGenBuffers(1, &backgroundVBO);
	glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundVerties), backgroundVerties, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(NULL);


	unsigned int NumberVAO, NumberVBO;
	glGenVertexArrays(1, &NumberVAO);
	glBindVertexArray(NumberVAO);

	glGenBuffers(1, &NumberVBO);
	glBindBuffer(GL_ARRAY_BUFFER, NumberVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 6, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	unsigned int textVAO;
	glGenVertexArrays(1, &textVAO);
	glBindVertexArray(textVAO);

	unsigned int textVBO;
	glGenBuffers(1, &textVBO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, NULL);

	glBindVertexArray(NULL);

	#pragma endregion

	Shader* backgroundShader = ResourcesManager::loadShader("Background Shaders", "vertex.vert", "fragment.frag");
	Shader* numberShader = ResourcesManager::loadShader("Number Shaders", "number_vertex.vert", "number_fragment.frag");
	Shader* textShader = ResourcesManager::loadShader("Text Shaders", "text_vertex.vert", "text_fragment.frag");
	ResourcesManager::loadCharacter("fonts\\ARLRDBD.TTF", 0, 48);
	ResourcesManager::loadTexture("background", "Images\\background1024.png", "tex");
	loadNumbers();

	glm::mat4 projection = glm::ortho<float>(0, 600.f, 0, 800.f);
	numberShader->use();
	numberShader->setMat4("projection", projection);
	textShader->use();
	textShader->setMat4("projection", projection);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		renderBackground(backgroundShader, backgroundVAO);

		bool bIsFinished; 
		bool bIsSuccess;
		auto gameState = game.getGameState(bIsFinished, bIsSuccess);

		renderNumber(numberShader, gameState, NumberVAO, NumberVBO);

		renderScore(textShader, textVAO, textVBO);
		renderBest(textShader, textVBO, textVBO);

		if (bIsFinished) {
			if (bIsSuccess) {
				Log::i("you win! Congratulations!");
			}
			else {
				Log::i("what a pity! you can press R to start a new game.");
			}
			break;
		}

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

#pragma region Function Definition
void processInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		game.pressRight();
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		game.pressLeft();
	}
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		game.pressUp();
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		game.pressDown();
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		GameMachine newGame;
		game = newGame;
	}
}

void loadNumbers() {
	for (int i = 2; i <= 2048; i *= 2) {
		std::string number = std::to_string(i);
		ResourcesManager::loadTexture(number, "Images\\numbers\\" + number + "_128.png", "number" + number);
	}
}

void renderBackground(Shader* shader, unsigned int VAO){
	shader->use();
	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ResourcesManager::getTexture("background")->getID());
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void renderNumber(Shader* shader, std::vector<std::vector<Point>> gameState, unsigned int VAO, unsigned int VBO) {
	shader->use();
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	for (int i = 3; i >= 0; i--) {
		for (int j = 0; j < 4; j++) {
			if (gameState[i][j].value == 0) {
				continue;
			}
			float numberVertices[] = {
				40 + 140 * j,		40 + 140 * (3 - i),			0.f, 0.f,
				40 + 140 * j,		40 + 140 * (3 - i) + 100,	0.f, 1.f,
				40 + 140 * j + 100,	40 + 140 * (3 - i) + 100,	1.f, 1.f,

				40 + 140 * j + 100,	40 + 140 * (3 - i) + 100,	1.f, 1.f,
				40 + 140 * j,		40 + 140 * (3 - i),			0.f, 0.f,
				40 + 140 * j + 100,	40 + 140 * (3 - i),			1.f, 0.f
			};

			glBindTexture(GL_TEXTURE_2D, ResourcesManager::getTexture(std::to_string(gameState[i][j].value))->getID());
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(numberVertices), numberVertices);
			glBindBuffer(GL_ARRAY_BUFFER, NULL);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
	glBindVertexArray(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void renderScore(Shader* shader, unsigned int VAO, unsigned int VBO)
{
	std::string score = std::to_string(game.getScore());
	renderText(shader, score, glm::vec3(0.f, 0.f, 0.f), VAO, VBO, 80, SCORE_Y, 1.f);
}

void renderBest(Shader* shader, unsigned int VAO, unsigned int VBO)
{
	std::string best = ResourcesManager::loadFile("Data\\best_score.dat");
	unsigned int bestScore = std::max(stoi(best), (int)game.getScore());

	renderText(shader, std::to_string(bestScore), glm::vec3(0.f, 0.f, 0.f), VAO, VBO, 420, SCORE_Y, 1.f);
	ResourcesManager::writeFile("Data\\best_score.dat", std::to_string(bestScore));
}

void renderText(Shader* shader, std::string text, glm::vec3 textColor, unsigned int VAO, unsigned int VBO, float x, float y, float scale) {
	shader->use();
	shader->setVector3("textColor", textColor);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	for (char c : text) {
		Character* ch = ResourcesManager::getSingleCharacter(c);
		if (!ch) {
			Log::e("character pointer is NULL");
			continue;
		}
		float xpos = x + ch->getBearing().x * scale;
		float ypos = y - (ch->getSize().y - ch->getBearing().y) * scale;

		float w = ch->getSize().x * scale;
		float h = ch->getSize().y * scale;
		float vertices[] = {
			xpos,     ypos + h,   0.0, 0.0,
			xpos,     ypos,       0.0, 1.0,
			xpos + w, ypos,       1.0, 1.0,

			xpos,     ypos + h,   0.0, 0.0,
			xpos + w, ypos,       1.0, 1.0,
			xpos + w, ypos + h,   1.0, 0.0
		};
		glBindTexture(GL_TEXTURE_2D, ch->getTextureID());
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch->getAdvance() >> 6) * scale; 
	}
	glBindVertexArray(NULL);
	glBindTexture(GL_TEXTURE_2D, NULL);
}
#pragma endregion