#include "Shader.h"
#include "Utils/Log.h"
#include <GL/glew.h>
#include "glm/gtc/type_ptr.hpp"

Shader::Shader(unsigned int ID)
{
	this->ID = ID;
}

unsigned int Shader::getID()
{
	return ID;
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBoolean(const char* paramName, bool param)
{
	glUniform1i(glGetUniformLocation(this->ID, paramName), param);
}

void Shader::setInt(const char* paramName, int param)
{
	glUniform1i(glGetUniformLocation(this->ID, paramName), param);
}

void Shader::setFloat(const char* paramName, float param)
{
	glUniform1f(glGetUniformLocation(this->ID, paramName), param);
}

void Shader::setVector2(const char* paramName, glm::vec2 param)
{
	glUniform2f(glGetUniformLocation(this->ID, paramName), param.x, param.y);
}

void Shader::setVector3(const char* paramName, glm::vec3 param)
{
	glUniform3f(glGetUniformLocation(this->ID, paramName), param.x, param.y, param.z);
}

void Shader::setMat3(const char* paramName, glm::mat3 param)
{
	glUniformMatrix3fv(glGetUniformLocation(this->ID, paramName), 1, GL_FALSE, glm::value_ptr(param));
}

void Shader::setMat4(const char* paramName, glm::mat4 param)
{
	glUniformMatrix4fv(glGetUniformLocation(this->ID, paramName), 1, GL_FALSE, glm::value_ptr(param));
}

void Shader::setTexture(const char* textureName, int textureSlot)
{
	setInt(textureName, textureSlot);
}

void Shader::checkCompileError(unsigned int ID, ShaderType type)
{
	 int success;
	char infoLog[512];

	if (type == ShaderType::OTHER) {
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			Log::e("program link error:\n" + std::string(infoLog));
		}
	} else {
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(ID, 512, NULL, infoLog);
			Log::e("shader compile error:\n" + std::string(infoLog));
		}
	}
}
