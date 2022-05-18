// Tell system that SDL is already handled
#define SDL_MAIN_HANDLED
//#define STB_IMAGE_IMPLEMENTATION

// Graphics libraries
#include "SDL2/SDL.h"
#include "GL/glew.h"
#include "wavefront/wavefront.h"

// System libraries
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/fwd.hpp>
#include <glm/gtx/compatibility.hpp>
#include <stdexcept>
#include <fstream>
#include <istream>
#include <iostream>
#include <string>
#include <vector>
#include "stb_image.h"


// Input file to return it as a string
std::string getFileCode(std::string fileName)
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
}

int main()
{

	// ======================================= WINDOW RENDER STUFF ======================================

	SDL_Window* window = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (!SDL_GL_CreateContext(window))
	{
		throw std::runtime_error("Failed to create OpenGL context");
	}

	// Initialise glew and check for errors
	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("Failed to create OpenGL context");
	}

	// ======================================= Render Curuthers the Cat =======================================

	WfModel cat = {0};

	WfModelLoad("curuthers/curuthers.obj", &cat);

	// ======================================= Preparing the Primitive Shape Data ======================================

	const GLfloat positions[] = {
	  0.0f, 0.5f, 0.0f,
	  -0.5f, -0.5f, 0.0f,
	  0.5f, -0.5f, 0.0f
	};

	// RGBA - Vector 4
	const GLfloat coords[] = {
	  0.5f, 0.0f,
	  0.0f, 1.0f,
	  1.0f, 1.0f,
	};

	GLuint positionsVboId = 0;

	// Create a new VBO on the GPU and bind it
	glGenBuffers(1, &positionsVboId);

	if (!positionsVboId)
	{
		throw std::exception();
	}

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);

	// Upload a copy of the data from memory into the new VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	// Reset the state
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint vaoId = 0;

	// Create a new VAO on the GPU and bind it
	glGenVertexArrays(1, &vaoId);

	if (!vaoId)
	{
		throw std::exception();
	}

	glBindVertexArray(vaoId);

	// Bind the position VBO, assign it to position 0 on the bound VAO
	// and flag it to be used
	glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		3 * sizeof(GLfloat), (void*)0);

	glEnableVertexAttribArray(0);

	// Reset the state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//MATRIX

	// COLOURS

	GLuint coloursVboId = 0;

	// Create a new VBO on the GPU and bind it
	glGenBuffers(1, &coloursVboId);

	if (!coloursVboId)
	{
		throw std::exception();
	}

	glBindBuffer(GL_ARRAY_BUFFER, coloursVboId);

	// Upload a copy of the data from memory into the new VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		0, (void*)0);

	glEnableVertexAttribArray(1);

	// Reset the state
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	// ======================================= Texturing Triangle =======================================

	int w = 0;
	int h = 0;

	//glm::mat4 model, projection, cam;
   // projection = glm::perspective(glm::radians(70.0f), (float)800 / (float)600, 0.1f, 100.0f);
	//cam = glm::mat4(1.0f);
	//cam = glm::translate(cam, -glm::vec3(0.0f, 0.0f, 0.0f));

	//model = glm::translate(model, glm::vec3(1.0f, 0.0f, 10.0f));

	/*unsigned char* data = stbi_load("image.png", &w, &h, NULL, 4);

	if (!data)
	{
		throw std::exception();
	}*/

	// Create and bind a texture.
	GLuint textureId = 0;
	glGenTextures(1, &textureId);

	if (!textureId)
	{
		throw std::exception();
	}

	glBindTexture(GL_TEXTURE_2D, textureId);

	// Upload the image data to the bound texture unit in the GPU
	/*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, data);*/

	// Free the loaded data because we now have a copy on the GPU
	//free(data);

	// Generate Mipmap so the texture can be mapped correctly
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind the texture because we are done operating on it
	glBindTexture(GL_TEXTURE_2D, 0);

	// ======================================= Preparing the Shader Program ======================================

	std::string vertContent = getFileCode("resources/light.vert");
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

	std::string fragmentShaderfile = getFileCode("resources/light.frag");
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
	GLuint programId = glCreateProgram();
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

	GLuint projectionLoc = glGetUniformLocation(programId, "u_Projection");
	GLuint viewLoc = glGetUniformLocation(programId, "u_View");
	GLuint modelLoc = glGetUniformLocation(programId, "u_Model");

	bool quit = false;

	glm::vec3 cat1Pos, cat2Pos, camPos, camOffset;

	cat1Pos = glm::vec3(-2.5f, 0.0f, -10.0f);
	cat2Pos = glm::vec3(2.5f, 0.0f, -10.0f);
	camOffset = glm::vec3(0.0f, 0.5f, 10.0f);

	float timeInAir = 0;
	bool canJump = true;

	while (!quit)
	{
		const Uint8* Keystate = SDL_GetKeyboardState(NULL);
		SDL_Event event = { 0 };

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		if (Keystate[SDL_SCANCODE_D])
		{
			cat1Pos += glm::vec3(0.05f, 0.0f, 0.0f);
		}
		if (Keystate[SDL_SCANCODE_A])
		{
			cat1Pos += glm::vec3(-0.05f, 0.0f, 0.0f);
		}
		if (Keystate[SDL_SCANCODE_S])
		{
			camOffset += glm::vec3(0.0f, 0.0f, 0.5f);
		}
		if (Keystate[SDL_SCANCODE_W])
		{
			camOffset += glm::vec3(0.0f, 0.0f, -0.5f);
		}
		if (Keystate[SDL_SCANCODE_SPACE] && timeInAir < 1.5 && canJump == true)
		{
			cat1Pos = glm::lerp(cat1Pos, glm::vec3(cat1Pos.x, 4, cat1Pos.z), 0.1f);
			timeInAir += 0.1;
		}
		else
		{
			canJump = false;
			timeInAir = 0;
			cat1Pos = glm::lerp(cat1Pos, glm::vec3(cat1Pos.x, 0, cat1Pos.z), 0.1f);
		}
		if (timeInAir >= 1.5)
		{
			canJump = false;
			timeInAir = 0;
			cat1Pos = glm::lerp(cat1Pos, glm::vec3(cat1Pos.x, 0, cat1Pos.z), 0.1f);
		}

		if (cat1Pos.y <= 0.2)
		{
			canJump = true;
		}

		if (camOffset.z <= 5)
		{
			camOffset = glm::vec3(camOffset.x, camOffset.y, 5.0f);
		}
		if (camOffset.z >= 15)
		{
			camOffset = glm::vec3(camOffset.x, camOffset.y, 15.0f);
		}

		camPos = cat1Pos + camOffset;

		glEnable(GL_CULL_FACE);
		// Drawing operation
		glDisable(GL_CULL_FACE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, cat.textureId);

		// Make sure current shader is bound
		//glUniform1i(samplerLoc, 1);

		// Clear red
		glClearColor(0, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		// Update buffers

		// ======================================= SUBMIT FOR DRAWING ======================================
		// Instruct OpenGL to use our shader program and our VAO

		//Cat 1

		glUseProgram(programId);

		glm::quat cat1Rot = glm::quat(glm::radians(glm::vec3(0.0f, 90.0f, 0.0f)));

		glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)800 / (float)600, 0.1f, 1000.0f);
		glm::mat4 cam = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);

		cam = glm::translate(cam, -camPos);

		//Rotate the model
		model = glm::translate(model, cat1Pos);
		model = model * glm::mat4_cast(cat1Rot);

		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(cat.vaoId);

		glEnable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLES, 0, cat.vertexCount);
		glDisable(GL_DEPTH_TEST);

		// Reset the state
		glBindVertexArray(0);
		glUseProgram(0);

		//Cat 2

		glUseProgram(programId);

		glm::quat cat2Rot = glm::quat(glm::radians(glm::vec3(0.0f, -90.0f, 0.0f)));

		model = glm::mat4(1.0f);
		model = glm::translate(model, cat2Pos);

		//Rotate the model
		model = model * glm::mat4_cast(cat2Rot);

		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(cat.vaoId);

		glEnable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLES, 0, cat.vertexCount);
		glDisable(GL_DEPTH_TEST);

		// Reset the state
		glBindVertexArray(0);
		glUseProgram(0);

		SDL_GL_SwapWindow(window);
	}
	return 0;
}