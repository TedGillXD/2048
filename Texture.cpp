#include "Texture.h"

Texture::Texture(unsigned int ID, std::string nameInShader)
{
	this->ID = ID;
	this->nameInShader = nameInShader;
}

unsigned int Texture::getID()
{
	return ID;
}

const char* Texture::getNameInShader()
{
	return nameInShader.c_str();
}
