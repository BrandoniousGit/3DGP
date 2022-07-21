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

#include "shader.h"

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

GLuint createFloor(const std::string& filePath)
{
	int w = 0;
	int h = 0;

	unsigned char* data = stbi_load(filePath.c_str(), &w, &h, NULL, 4);
	
	if (!data)
	{
		throw std::runtime_error("It brokey data");
	}

	GLuint textId = 0;
	glGenTextures(1, &textId);

	if (!textId)
	{
		throw std::runtime_error("It brokey textId");
	}

	glBindTexture(GL_TEXTURE_2D, textId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	free(data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return textId;
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

	WfModel cat = { 0 };

	WfModelLoad("curuthers/curuthers.obj", &cat);

	WfModel floor = { 0 };

	WfModelLoad("floor/WelcomeMatOBJ.obj", &floor);

	floor.textureId = createFloor("floor/Textures/WelcomeMatClear_diffuse.jpg");

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

	Shader shader("resources/light.vert", "resources/light.frag");

	GLuint projectionLoc = glGetUniformLocation(shader.getProgId(), "u_Projection");
	GLuint viewLoc = glGetUniformLocation(shader.getProgId(), "u_View");
	GLuint modelLoc = glGetUniformLocation(shader.getProgId(), "u_Model");

	bool quit = false;

	glm::vec3 cat1Pos, cat2Pos, camPos, camOffset, floorPos;

	glm::quat camRot = glm::quat(glm::radians(glm::vec3(0.0f, 0.0f, 0.0f)));

	//cat1Pos = glm::vec3(-2.5f, 0.0f, -10.0f);
	cat1Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	cat2Pos = glm::vec3(0.0f, -3.33f, -1.0f);
	floorPos = glm::vec3(0.0f, -4.5f, -10.0f);
	camOffset = glm::vec3(0.0f, 0.0f, 0.0f);

	float timeInAir = 0;
	float angle = 0, offset = 10;

	bool canJump = true;

	while (!quit)
	{
		const Uint8* Keystate = SDL_GetKeyboardState(NULL);
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		cat2Pos += glm::vec3(-0.15f, 0, 0);

		if (cat2Pos.x <= -20.0f)
		{
			cat2Pos = glm::vec3(10.0f, cat2Pos.y, cat2Pos.z);
		}

		//For moving the player left and right
		/*if (Keystate[SDL_SCANCODE_D])
		{
			cat1Pos += glm::vec3(0.05f, 0.0f, 0.0f);
		}
		if (Keystate[SDL_SCANCODE_A])
		{
			cat1Pos += glm::vec3(-0.05f, 0.0f, 0.0f);
		}*/

		//camRot *= glm::quat(glm::radians(glm::vec3(0.0f, glm::radians(-55.0f), 0.0f)));

		//For zooming the camera in and out
		if (Keystate[SDL_SCANCODE_S])
		{
			offset += 0.1f;
		}
		if (Keystate[SDL_SCANCODE_W])
		{
			offset -= 0.1f;
		}

		//Holding the "O" key will orbit the cat model
		if (Keystate[SDL_SCANCODE_O])
		{
			angle += 1.0f;
		}

		camOffset = glm::vec3(sin(glm::radians(angle)) * offset, 0.0f, cos(glm::radians(angle)) * offset);

		//Jumping using lerp
		if (Keystate[SDL_SCANCODE_SPACE] && timeInAir < 2.4 && canJump == true)
		{
			cat1Pos = glm::lerp(cat1Pos, glm::vec3(cat1Pos.x, 4, cat1Pos.z), 0.1f);
			timeInAir += 0.1;
		}
		else
		{
			canJump = false;
			timeInAir = 0;
			cat1Pos = glm::lerp(cat1Pos, glm::vec3(cat1Pos.x, 0, cat1Pos.z), 0.07f);
		}
		if (timeInAir >= 2.4)
		{
			canJump = false;
			timeInAir = 0;
			cat1Pos = glm::lerp(cat1Pos, glm::vec3(cat1Pos.x, 0, cat1Pos.z), 0.07f);
		}

		if (cat1Pos.y <= 0.2)
		{
			canJump = true;
		}


		//Locking the camera offset between 5 and 15 units
		if (offset <= 5)
		{
			offset = 5;
		}
		if (offset >= 15)
		{
			offset = 15;
		}

		//Applying the camera position
		camPos = camOffset + glm::vec3(cat1Pos.x, 0, cat1Pos.z);

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

		glUseProgram(shader.getProgId());

		glm::quat cat1Rot = glm::quat(glm::radians(glm::vec3(0.0f, 90.0f, 0.0f)));

		glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)800 / (float)600, 0.1f, 1000.0f);
		glm::mat4 cam = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);

		//cam = cam * glm::mat4_cast(camRot);
		cam = glm::lookAt(camPos, glm::vec3(cat1Pos.x, 0, cat1Pos.z), glm::vec3(0.0f, 1.0f, 0.0f));
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

		glUseProgram(shader.getProgId());

		glm::quat cat2Rot = glm::quat(glm::radians(glm::vec3(0.0f, -70.0f, -90.0f)));
		glm::vec3 cat2Scale = glm::vec3(2.0f, 2.0f, 2.0f);

		model = glm::mat4(1.0f);
		model = glm::translate(model, cat2Pos);
		model = glm::scale(model, cat2Scale);

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

		//Floor
		glUseProgram(shader.getProgId());
		glBindTexture(GL_TEXTURE_2D, floor.textureId);

		model = glm::mat4(1.0f);
		model = glm::translate(model, floorPos);

		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(floor.vaoId);

		glEnable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLES, 0, floor.vertexCount);
		glDisable(GL_DEPTH_TEST);

		// Reset the state
		glBindVertexArray(0);
		glUseProgram(0);

		SDL_GL_SwapWindow(window);
	}
	return 0;
}