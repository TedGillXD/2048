#pragma once
#include "glm/glm.hpp"

enum class ShaderType {
	VERTEX,
	FRAGMENT,
	GEOMETRY,
	OTHER
};

class Shader
{
private:
	unsigned int ID;

public:
	Shader(unsigned int ID);

	unsigned int getID();

	void use();

	void setBoolean(const char* paramName, bool param);
	void setInt(const char* paramName, int param);
	void setFloat(const char* paramName, float param);
	void setVector2(const char* paramName, glm::vec2 param);
	void setVector3(const char* paramName, glm::vec3 param);
	void setMat3(const char* paramName, glm::mat3 param);
	void setMat4(const char* paramName, glm::mat4 param);
	void setTexture(const char* textureName, int textureSlot);

	static void checkCompileError(unsigned int ID, ShaderType type);
};

