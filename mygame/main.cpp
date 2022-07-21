// Tell system that SDL is already handled
#define SDL_MAIN_HANDLED
//#define STB_IMAGE_IMPLEMENTATION

// Graphics libraries
#include "SDL2/SDL.h"
#include "GL/glew.h"

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

//External header files
#include "shader.h"
#include "stb_image.h"
#include "gameobject.h"

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

	std::vector<GameObject> gameObjects;

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

	// Create and bind a texture.
	GLuint textureId = 0;
	glGenTextures(1, &textureId);

	if (!textureId)
	{
		throw std::exception();
	}

	glBindTexture(GL_TEXTURE_2D, textureId);

	// Generate Mipmap so the texture can be mapped correctly
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind the texture because we are done operating on it
	glBindTexture(GL_TEXTURE_2D, 0);

	Shader shader("resources/light.vert", "resources/light.frag");

	//Adding game objects to the array
	gameObjects.push_back(GameObject("curuthers/curuthers.obj", shader, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(180.0f, 270.0f, 0.0f), glm::vec3(1.0f)));
	gameObjects.push_back(GameObject("curuthers/curuthers.obj", shader, glm::vec3(-5.0f, -3.38f, -1.0f), glm::vec3(180.0f, 90.0f, 270.0f), glm::vec3(2.0f)));
	gameObjects.push_back(GameObject("floor/WelcomeMatOBJ.obj", shader, glm::vec3(0.0f, -4.5f, -10.0f), glm::vec3(180.0f, 0.0f, 0.0f), glm::vec3(1.0f)));

	//Giving the welcome mat the correct texture
	gameObjects[2].SetTexture("floor/Textures/WelcomeMatClear_diffuse.jpg");

	bool quit = false;

	//Variable declaring
	glm::vec3 camPos, camOffset;

	glm::quat camRot = glm::quat(glm::radians(glm::vec3(0.0f, 0.0f, 0.0f)));

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

	    gameObjects[1].Translate(glm::vec3(-0.15f, 0, 0));

		//Moves obstacle across screen, resets at -20
		if (gameObjects[1].GetPosition().x <= -20.0f)
		{
			gameObjects[1].SetPosition(glm::vec3(10.0f, gameObjects[1].GetPosition().y, gameObjects[1].GetPosition().z));
		}

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
		else if (Keystate[SDL_SCANCODE_P])
		{
			angle -= 1.0f;
		}

		camOffset = glm::vec3(sin(glm::radians(angle)) * offset, 0.0f, cos(glm::radians(angle)) * offset);

		//Jumping using lerp
		if (Keystate[SDL_SCANCODE_SPACE] && timeInAir < 2.4 && canJump == true)
		{
			gameObjects[0].SetPosition(glm::lerp(gameObjects[0].GetPosition(), glm::vec3(gameObjects[0].GetPosition().x, 4, gameObjects[0].GetPosition().z), 0.1f));
			timeInAir += 0.1f;
		}
		else
		{
			canJump = false;
			timeInAir = 0;
			gameObjects[0].SetPosition(glm::lerp(gameObjects[0].GetPosition(), glm::vec3(gameObjects[0].GetPosition().x, 0, gameObjects[0].GetPosition().z), 0.07f));
		}
		if (timeInAir >= 2.4)
		{
			canJump = false;
			timeInAir = 0;
			gameObjects[0].SetPosition(glm::lerp(gameObjects[0].GetPosition(), glm::vec3(gameObjects[0].GetPosition().x, 0, gameObjects[0].GetPosition().z), 0.07f));
		}

		if (gameObjects[0].GetPosition().y <= 0.2)
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
		camPos = camOffset + glm::vec3(gameObjects[0].GetPosition().x, 0, gameObjects[0].GetPosition().z);

		glEnable(GL_CULL_FACE);

		// Drawing operation
		glDisable(GL_CULL_FACE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Clear red
		glClearColor(0, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		// Update buffers

		glm::mat4 viewMat;

		viewMat = glm::lookAt(camPos, glm::vec3(gameObjects[0].GetPosition().x, 0, gameObjects[0].GetPosition().z), glm::vec3(0.0f, 1.0f, 0.0f));
		viewMat = glm::translate(viewMat, -camPos);


		//For loop to draw gameobjects into scene
		glEnable(GL_DEPTH_TEST);
		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i].DrawGameObject(shader, window, glm::vec3(0.0f), viewMat);
		}
		glDisable(GL_DEPTH_TEST);

		SDL_GL_SwapWindow(window);
	}
	return 0;
}