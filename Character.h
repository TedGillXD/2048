#pragma once
#include <iostream>
#include <map>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Character
{
private:
	unsigned int textureID;
	glm::ivec2 size;		//character size
	glm::ivec2 bearing;		
	unsigned int advance;	//The distance between two adjacent digits

public:
	Character(unsigned int textureID, glm::ivec2 size, glm::ivec2 bearing, unsigned int advance);

	unsigned int getTextureID();

	glm::ivec2 getSize();

	glm::ivec2 getBearing();

	unsigned int getAdvance();

};
