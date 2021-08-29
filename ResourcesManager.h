#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include "Shader.h"
#include <fmt/format.h>
#include "Utils/Log.h"

std::map<std::string, Shader*> shaderList;

class ResourcesManager
{
private:

public:
	static Shader* getShader(std::string name) {
		if (shaderList.find(name) != shaderList.end()) {
			return shaderList[name];
		}
		return NULL;
	}

	static Shader* loadShader(std::string name, std::string vertexShaderPath, std::string fragmentShaderPath) {
		if (getShader(name)) {
			Log::e(fmt::format("shader named {0} is already exist", name));
			return NULL;
		}

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

			const char* vertexSource = vertexStream.str().c_str();
			const char* fragmentSource = fragmentStream.str().c_str();

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
		if (getShader(name)) {
			Log::e(fmt::format("shader named {0} is already exist", name));
			return NULL;
		}

		std::ifstream vertexIfs;
		std::ifstream geometryIfs;
		std::ifstream fragmentIfs;

		std::stringstream vertexStream;
		std::stringstream fragmentStream;
		std::stringstream geometryStream;

		std::string fileDir = "shader\\";

		vertexIfs.open((fileDir + vertexShaderPath).c_str());
		geometryIfs.open((fileDir + geometryShaderPath).c_str());
		fragmentIfs.open((fileDir + fragmentShaderPath).c_str());

		vertexIfs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentIfs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		geometryIfs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			if (!vertexIfs.is_open()) {
				throw std::exception("open vertex file fail");
			}
			if (!fragmentIfs.is_open()) {
				throw std::exception("open fragment file fail");
			}
			if (!geometryIfs.is_open()) {
				throw std::exception("open geometry file fail");
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
};