#pragma once
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <fmt/format.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ft2build.h";
#include FT_FREETYPE_H

#include "Character.h"
#include "Log.h"
#include "Shader.h"
#include "Texture.h"

class ResourcesManager
{
private:
	static std::map<std::string, Shader*> shaderList;

	static std::map<std::string, Texture*> textureList;

	static std::map<char, Character*> characterList;

	template<class T1, class T2>
	static std::map<T1, T2*> createMap() {
		return std::map<T1, T2*>();
	}

public:
	static Shader* getShader(std::string name) {
		if (shaderList.find(name) != shaderList.end()) {
			return shaderList[name];
		}
		return NULL;
	}

	static Shader* loadShader(std::string name, std::string vertexShaderPath, std::string fragmentShaderPath) {
		std::ifstream vertexIfs;
		std::ifstream fragmentIfs;
		std::stringstream vertexStream;
		std::stringstream fragmentStream;

		std::string fileDir = "Shaders\\";

		vertexIfs.open((fileDir + vertexShaderPath).c_str());
		fragmentIfs.open((fileDir + fragmentShaderPath).c_str());
		vertexIfs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentIfs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			if (!vertexIfs.is_open()) {
				throw std::exception("fail to open vertex file");
			}
			if (!fragmentIfs.is_open()) {
				throw std::exception("fail to open fragment file");
			}

			//rdbuf == read buffer
			vertexStream << vertexIfs.rdbuf();
			fragmentStream << fragmentIfs.rdbuf();

			std::string vertexString = vertexStream.str();
			std::string fragmentString = fragmentStream.str();

			const char* vertexSource = vertexString.c_str();
			const char* fragmentSource = fragmentString.c_str();

			unsigned int vertex, fragment;

			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vertexSource, NULL);
			glCompileShader(vertex);
			Shader::checkCompileError(vertex, ShaderType::VERTEX);

			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fragmentSource, NULL);
			glCompileShader(fragment);
			Shader::checkCompileError(fragment, ShaderType::FRAGMENT);

			unsigned int ID = glCreateProgram();
			glAttachShader(ID, vertex);
			glAttachShader(ID, fragment);
			glLinkProgram(ID);
			Shader::checkCompileError(ID, ShaderType::OTHER);

			Shader* shader = new Shader(ID);
			shaderList[name] = shader;
			return shader;
		}
		catch (std::exception& e)
		{
			Log::e(e.what());
		}
		return NULL;
	}

	static Shader* loadShader(std::string name, std::string vertexShaderPath, std::string geometryShaderPath, std::string fragmentShaderPath) {
		std::ifstream vertexIfs;
		std::ifstream geometryIfs;
		std::ifstream fragmentIfs;

		std::stringstream vertexStream;
		std::stringstream fragmentStream;
		std::stringstream geometryStream;

		std::string fileDir = "Shaders\\";

		vertexIfs.open((fileDir + vertexShaderPath).c_str());
		geometryIfs.open((fileDir + geometryShaderPath).c_str());
		fragmentIfs.open((fileDir + fragmentShaderPath).c_str());

		vertexIfs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentIfs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		geometryIfs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			if (!vertexIfs.is_open()) {
				throw std::exception("fail to open vertex file");
			}
			if (!fragmentIfs.is_open()) {
				throw std::exception("fail to open fragment file");
			}
			if (!geometryIfs.is_open()) {
				throw std::exception("fail to open geometry file");
			}

			vertexStream << vertexIfs.rdbuf();
			fragmentStream << fragmentIfs.rdbuf();
			geometryStream << geometryIfs.rdbuf();

			const char* vertexSource = vertexStream.str().c_str();
			const char* fragmentSource = fragmentStream.str().c_str();
			const char* geometrySource = geometryStream.str().c_str();

			unsigned int vertex, fragment, geometry;

			vertex = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex, 1, &vertexSource, NULL);
			glCompileShader(vertex);
			Shader::checkCompileError(vertex, ShaderType::VERTEX);

			fragment = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment, 1, &fragmentSource, NULL);
			glCompileShader(fragment);
			Shader::checkCompileError(fragment, ShaderType::FRAGMENT);

			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &geometrySource, NULL);
			glCompileShader(geometry);
			Shader::checkCompileError(geometry, ShaderType::GEOMETRY);

			unsigned int ID = glCreateProgram();
			glAttachShader(ID, vertex);
			glAttachShader(ID, fragment);
			glAttachShader(ID, geometry);
			glLinkProgram(ID);
			Shader::checkCompileError(ID, ShaderType::OTHER);

			Shader* shader = new Shader(ID);
			shaderList[name] = shader;
			return shader;
		}
		catch (std::exception& e)
		{
			Log::e(e.what());
		}

		return NULL;
	}

	static Texture* loadTexture(std::string name, std::string texturePath, std::string nameInShader) {
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);
		if (data) {
			GLenum format;
			if (channels == 1)
				format = GL_RED;
			else if (channels == 3)
				format = GL_RGB;
			else if (channels == 4)
				format = GL_RGBA;
			
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else {
			Log::e("Texture failed to load at path: " + texturePath);
			stbi_image_free(data);
		}

		Texture* texture = new Texture(textureID, nameInShader);
		textureList[name] = texture;
		return texture;
	}

	static Texture* getTexture(std::string name) {
		if (textureList.find(name) != textureList.end()) {
			return textureList[name];
		}
		return NULL;
	}

	static std::map<char, Character*> loadCharacter(std::string ttfPath, int charWidth, int charHeight) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			Log::e("fail to init FT_Library");
			return characterList;
		}

		FT_Face face;
		if (FT_New_Face(ft, ttfPath.c_str(), 0, &face)) {
			Log::e("fail to init FT_Face");
			return characterList;
		}

		FT_Set_Pixel_Sizes(face, charWidth, charHeight);

		for (GLubyte c = 0; c < 128; c++) {
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				Log::e(fmt::format("fail to load {0} Glyph", c));
				continue;
			}
			int width = face->glyph->bitmap.width;
			int height = face->glyph->bitmap.rows;
			unsigned int textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			int bearingX = face->glyph->bitmap_left;
			int bearingY = face->glyph->bitmap_top;
			unsigned int advance = face->glyph->advance.x;
			Character* character = new Character(textureID, glm::ivec2(width, height), glm::ivec2(bearingX, bearingY), advance);
			characterList[c] = character;
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
		return characterList;
	}

	static Character* getSingleCharacter(char c) {
		if (characterList.find(c) != characterList.end()) {
			return characterList[c];
		}
		return NULL;
	}

	static std::string loadFile(std::string filePath) {

		std::ifstream ifs;
		ifs.open(filePath.c_str());
		if (!ifs.is_open()) {
			return "";
		}

		std::stringstream ss;
		ss << ifs.rdbuf();

		ifs.close();
		return ss.str();
	}

	static bool writeFile(std::string filePath, std::string data, std::ios_base::open_mode mode = std::ios::out | std::ios::trunc) {
		std::ofstream ofs;
		ofs.open(filePath.c_str(), mode);

		if (!ofs.is_open()) {
			ofs.close();
			return false;
		}

		ofs << data.c_str();

		ofs.close();
		return true;
	}
};

std::map<std::string, Shader*> ResourcesManager::shaderList = ResourcesManager::createMap<std::string, Shader>();
std::map<std::string, Texture*> ResourcesManager::textureList = ResourcesManager::createMap<std::string, Texture>();
std::map<char, Character*> ResourcesManager::characterList = ResourcesManager::createMap<char, Character>(); 