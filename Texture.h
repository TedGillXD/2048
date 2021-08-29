#pragma once
#include <string>

class Texture
{
private:
	unsigned int ID;

	std::string nameInShader;

public:
	Texture(unsigned int ID, std::string nameInShader);

	unsigned int getID();

	const char* getNameInShader();

};

