#pragma once
#include <fstream>
#include <string>
#include <GL/glew.h>

struct Shader
{
	Shader(const std::string& _vertexPath, const std::string& _fragmentPath);

	void Use();
	void NoUse();
	GLuint getProgId();

private:
	GLuint programId;
};