#include "Character.h"

Character::Character(unsigned int textureID, glm::ivec2 size, glm::ivec2 bearing, unsigned int advance)
{
	this->advance = advance;
	this->bearing = bearing;
	this->size = size;
	this->textureID = textureID;
}

unsigned int Character::getTextureID()
{
	return textureID;
}

glm::ivec2 Character::getSize()
{
	return size;
}

glm::ivec2 Character::getBearing()
{
	return bearing;
}

unsigned int Character::getAdvance()
{
	return advance;
}