#include "shader.h"

/*std::string getFileCode(std::string fileName)
{
	std::string line;
	std::string fileContent;
	std::ifstream myfile(fileName);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			fileContent += line;
		}

		myfile.close();
	}
	return fileContent;
}*/

std::string load_file(const std::string& _path)
{
	std::ifstream file(_path.c_str());
	if (!file.is_open())
	{
		throw std::runtime_error("File failed to open");
	}

	std::string rtn;
	std::string line;

	while (!file.eof())
	{

		std::getline(file, line);
		rtn += line + '\n';
	}

	return rtn;
}

Shader::Shader(const std::string& _vertexPath, const std::string& _fragmentPath)
{
	std::string vertContent = load_file(_vertexPath);
	const GLchar* vertexShaderSrc = vertContent.c_str();
	// Create a new vertex shader, attach source code, compile it and
	// check for errors.
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShaderId);
	GLint success = 0;
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		throw std::exception();
	}

	std::string fragmentShaderfile = load_file(_fragmentPath);
	const GLchar* fragmentShaderSrc = fragmentShaderfile.c_str();

	// Create a new fragment shader, attach source code, compile it and
	// check for errors.
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShaderId);
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		throw std::exception();
	}

	// Create new shader program and attach our shader objects
	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	// Ensure the VAO "position" attribute stream gets set as the first position
	// during the link.
	glBindAttribLocation(programId, 0, "a_Position");
	glBindAttribLocation(programId, 1, "a_TexCoord");
	glBindAttribLocation(programId, 2, "a_Normal");

	// Perform the link and check for failure
	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &success);

	if (!success)
	{
		throw std::exception();
	}

	// Detach and destroy the shader objects. These are no longer needed
	// because we now have a complete shader program.
	glDetachShader(programId, vertexShaderId);
	glDeleteShader(vertexShaderId);
	glDetachShader(programId, fragmentShaderId);
	glDeleteShader(fragmentShaderId);
}

GLuint Shader::getProgId()
{
	return programId;
}

void Shader::Use()
{
	glUseProgram(programId);
}

void Shader::NoUse()
{
	glUseProgram(0);
}