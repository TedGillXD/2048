#include "Mesh.h"

Mesh::Mesh(float vertices[])
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(NULL);
}


void Mesh::addTexture(Texture* texture)
{
	textures.push_back(texture);
}

void Mesh::draw(Shader* shader, GLenum mode, GLint first, GLint count)
{
	glBindVertexArray(VAO);

	for (int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]->getID());
		shader->setTexture(textures[i]->getNameInShader(), i);
	}

	glDrawArrays(mode, first, count);

	glBindVertexArray(NULL);
}
