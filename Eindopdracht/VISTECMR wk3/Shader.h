#pragma once

#include <map>
#include <string>

#include <GL/glew.h>
#include <glm.hpp>

class Shader
{
	std::map<std::string, GLuint> uniforms;
	GLuint programId = -1;
	bool hasGeometry;
public:
	std::string name;

	//Shader(const std::string &filename);
	Shader(const std::string &filename, bool hasGeometry = false);

	inline bool isValid()	{		return programId != -1;	}

	void checkForUpdate();

	void reloadShaders();
	void reloadDefaultShaders();
	void reloadGeometryShaders();

	void use();

	GLuint getUniform(const std::string &name);

	void setUniform(const std::string &name, const glm::mat4 &mat);
	void setUniform(const std::string &name, int val);
};