#pragma once
#include <vector>
#include "Texture.h"
#include "Shader.h"
#include "gl/glew.h"

class Mesh
{
private:
	unsigned int VAO;
	unsigned int VBO;

	std::vector<Texture*> textures;

public:
	Mesh(float vertices[]);

	void addTexture(Texture* texture);

	void draw(Shader* shader, GLenum mode, GLint first, GLint count);
};

